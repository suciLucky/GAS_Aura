// Copyright JCS


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"


//制作所捕获的属性变量
struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)

	//将GameplayTags映射到捕获的属性
	TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitChance,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitDamage,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitResistance,Target,false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,FireResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,LightningResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArcaneResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,PhysicalResistance,Target,false);

		const FAuraGameplayTags Tags = FAuraGameplayTags::Get();
		
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor,ArmorDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration,ArmorPenetrationDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance,BlockChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance,CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage,CriticalHitDamageDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance,CriticalHitResistanceDef);


		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire,FireResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning,LightningResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane,ArcaneResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical,PhysicalResistanceDef);
	}	
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if(SourceAvatar->Implements<UCombatInterface>())
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);

	int32 TargetPlayerLevel = 1;
	if(TargetAvatar->Implements<UCombatInterface>())
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);		
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTag;
	EvaluateParameters.TargetTags = TargetTag;

	//Get Damage Set by Caller Magnitude from Tag
	float Damage = 0.f;
	for (auto  Pair:FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		checkf(AuraDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag),TEXT("TagsToCaptureDefs doesn't contain Tag:[%s]) in ExecCalc_Damage"),*ResistanceTag.ToString());

        float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag,false);
		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = AuraDamageStatics().TagsToCaptureDefs[ResistanceTag];
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition,EvaluateParameters,Resistance);
		Resistance = FMath::Clamp(Resistance,0.f,100.f);		
		
		DamageTypeValue *= (100.f-Resistance)/100.f; 
		Damage += DamageTypeValue;
	}

	//Capture BlockChance on Target,and determine if there was a successful block.	
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluateParameters,TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.f,TargetBlockChance);
	const bool bBlocked = FMath::RandRange(1,100)<TargetBlockChance;

	//设置Context上的bBlocked
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
    UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle,bBlocked);
	
	//If blocked ,have damage.
	Damage = bBlocked ? (Damage/2.f) : Damage;

	//Capture Armor on Target
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParameters,TargetArmor);
	TargetArmor = FMath::Max<float>(0.f,TargetArmor);
	
	//Capture ArmorPenetration on Source,will ignore a percentage of Target's Armor.
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluateParameters,SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(0.f,SourceArmorPenetration);

	//获取计算系数
	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCoeCurve =  CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetrationCoe"),FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCoeCurve->Eval(SourcePlayerLevel);
	const FRealCurve*EffectiveArmorCoeCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmorCoe"),FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCoeCurve->Eval(TargetPlayerLevel);
	const FRealCurve*CriticalHitResistanceCoeCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistanceCoe"),FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCoeCurve->Eval(TargetPlayerLevel);
    
	//捕获源暴击率
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,EvaluateParameters,SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0.f,SourceCriticalHitChance);
	//捕获源暴击伤害
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef,EvaluateParameters,SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0.f,SourceCriticalHitDamage);	
	//捕获目标暴击抗性
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics(). CriticalHitResistanceDef,EvaluateParameters,TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(0.f,TargetCriticalHitResistance);
	//计算实际暴击率
	const float EffectiveCriticalHitChance = SourceCriticalHitChance-TargetCriticalHitResistance*CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1,100)<EffectiveCriticalHitChance;
	//设置Context的是否暴击
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle,bCriticalHit);
	//计算暴击与否的伤害
	Damage = bCriticalHit ? Damage*2+SourceCriticalHitDamage : Damage;
	
	//剩余的护甲,减少伤害
	const float EffectiveArmor = TargetArmor*(100.f-SourceArmorPenetration*ArmorPenetrationCoefficient)/100.f;
	Damage*=(100-EffectiveArmor*EffectiveArmorCoefficient)/100;
	
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
