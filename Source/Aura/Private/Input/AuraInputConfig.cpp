// Copyright JCS


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound)
{
	for(const auto& Action:AbilityInputActions)
	{
		if(Action.InputAction&&Action.InputTag==InputTag)
		{
			return Action.InputAction;
		}
	}
	if(bLogNotFound)
	{
		UE_LOG(LogTemp,Error,TEXT("Can not find AbilityInputAction for InputTag [%s],on InputConfig [%s]"),*InputTag.ToString(),*GetNameSafe(this));
	}
	return nullptr;
}
