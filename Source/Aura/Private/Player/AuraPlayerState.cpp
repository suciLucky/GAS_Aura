// Copyright JCS


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	//构造函数初始创建AbilitySystemComponent和AttributeSet
	AbilitySystemComponent=CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	
	AttributeSet=CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	//设置玩家的复制模式为Mixed，意味着Gameplay Effects are	replicated to the owning client only. Gameplay Cues and Gameplay Tags replicated to all clients.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	NetUpdateFrequency=100.f; //服务器向所有客户端更新数据的频率
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//注册
	DOREPLIFETIME(AAuraPlayerState,Level);
}

//获取技能组件函数的定义
UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
}
