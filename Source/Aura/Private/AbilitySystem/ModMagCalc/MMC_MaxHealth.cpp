// Copyright JCS


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture=UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource=EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot=false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//获取源标签和目标标签
	const FGameplayTagContainer* SourceTag=Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag=Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags=SourceTag;
	EvaluationParameters.TargetTags=TargetTag;

	float Vigor=0.f;
	//获取属性数值
	GetCapturedAttributeMagnitude(VigorDef,Spec,EvaluationParameters,Vigor);
	//夹值到0
	Vigor=FMath::Max(Vigor,0.f);

	//获得PlayerLevel
	int32 PlayerLevel = 1;
	if(Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());;

	//最终计算
	return 80.f+Vigor*2.5f+10.f*PlayerLevel;
}
