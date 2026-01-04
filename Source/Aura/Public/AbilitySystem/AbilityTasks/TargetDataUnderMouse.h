// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Algo/Compare.h"
#include "TargetDataUnderMouse.generated.h"

//
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature ,const FGameplayAbilityTargetDataHandle&,DataHandle);
/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
public:
	//一个创建该任务实例的函数
	UFUNCTION(BlueprintCallable,Category="Ability|Tasks",meta=(DisplayName = "TargetDataUnderMouse",HidePin = "OwningAbility",
		DefaultToSelf= "OwningAbility",BlueprintInternalUseOnly="true"))
	static UTargetDataUnderMouse* CreatTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	//异步执行引脚，通过委托实现
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;
private:
	//在任务激活时广播Data
	virtual void Activate() override;
	//发送数据到服务器
	void SendMouseCursorData();
	//服务器接收到数据时触发的回调函数
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,FGameplayTag ActivationTag);
};
