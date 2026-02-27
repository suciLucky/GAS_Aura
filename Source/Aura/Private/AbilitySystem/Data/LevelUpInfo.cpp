// Copyright JCS


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(const int32& XP) const
{	
	for (int32 i = 1;i < LevelUpInformation.Num();++i)			
		if(XP < LevelUpInformation[i].LevelUpRequirement)
			return i;			
	return LevelUpInformation.Num()-1;
}
