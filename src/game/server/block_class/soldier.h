#ifndef GAME_SERVER_BLOCK_CLASS_SOLDIER_H
#define GAME_SERVER_BLOCK_CLASS_SOLDIER_H

#include "block_class.h"

class CBlockClassSoldier final : public IBlockClass
{
public:
	const char *CommandName() const override;
	const char *DisplayName() const override;
	const char *Description() const override;

	void OnExplosionHit(CGameContext *pGameServer, int Weapon, CCharacter *pOwner, CCharacter *pTarget, vec2 HitPos, vec2 ForceDir) override;

private:
	int m_FreezeSeconds = 3;
	float m_Knockback = 18.0f;
};

#endif
