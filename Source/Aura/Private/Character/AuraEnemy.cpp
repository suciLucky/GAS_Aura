// Copyright JCS


#include "Character/AuraEnemy.h"

#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	//初始化碰撞通道
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
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
