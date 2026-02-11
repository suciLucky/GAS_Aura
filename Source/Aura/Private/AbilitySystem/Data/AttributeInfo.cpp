// Copyright JCS


#include "AbilitySystem/Data/AttributeInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag,const bool bLogNotFound) const
{
	for(auto Info:AttributeInformation)
	{
		if(Info.AttributeTag==AttributeTag)
		{
			return Info;
		}
	}
	if(bLogNotFound)
	{
		UE_LOG(LogAura,Error,TEXT("Can not find AttributrTag [%s] in AttibuteInfo [%s]"),*AttributeTag.ToString(),*GetNameSafe(this));
	}
	return FAuraAttributeInfo();
}
