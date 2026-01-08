// Copyright JCS


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraEnemy::AAuraEnemy()
{
	//初始化碰撞通道
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	//定义Enemy的AbilitySystemComponent和AttributeSet
	AbilitySystemComponent=CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet=CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	//设置玩家的复制模式为Minimal,意味着Gameplay Effects are not replicated. Gameplay Cues and Gameplay Tags replicated to all clients.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	EnemyHealthBar= CreateDefaultSubobject<UWidgetComponent>("EnemyHealthBar");
	EnemyHealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	//初始化技能
	UAuraAbilitySystemLibrary::GiveStartupAbilities(this,AbilitySystemComponent);
	
	//设置血条的Controller为敌人自身
	if(UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(EnemyHealthBar->GetUserWidgetObject()))
		AuraUserWidget->SetWidgetController(this);
	
	//将委托绑定到Lambda函数（广播新值）		
	if(const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
     	{
     		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
     			[this](const FOnAttributeChangeData& Data)
     			{
     					OnHealthChanged.Broadcast(Data.NewValue);
     			});
     		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
     			[this,AuraAttributeSet](const FOnAttributeChangeData& Data)
     			{
     				OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
     			});
            //注册受击标签变化时的监听事件并绑定回调函数
		    AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact,EGameplayTagEventType::NewOrRemoved).AddUObject(
		    this,&AAuraEnemy::HitReactTagChanged);
     		//广播初始值
     		OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
     		OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
     	}	
}
void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0.f;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	InitializeDefaultAttributes();
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this,CharacterClass,Level,AbilitySystemComponent);
}


/**关卡使用无限大的后期处理体积，在后期处理材质添加物体高亮材质（基于自定义深度模板通道），设置需要高亮物体的RenderCustomDepth
*和CustomDepthStencilValue来控制高亮
**/
void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}


