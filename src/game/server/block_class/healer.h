#ifndef GAME_SERVER_BLOCK_CLASS_HEALER_H
#define GAME_SERVER_BLOCK_CLASS_HEALER_H

#include "block_class.h"

class CBlockClassHealer final : public IBlockClass
{
public:
	const char *CommandName() const override;
	const char *DisplayName() const override;
	const char *Description() const override;

	float AdjustWeaponFireDelay(CGameContext *pGameServer, CCharacter *pCharacter, int Weapon, float BaseDelay) const override;
	void OnProjectileHit(CGameContext *pGameServer, int Weapon, CCharacter *pOwner, CCharacter *pTarget, vec2 HitPos) override;

private:
	float m_GunFireDelayMultiplier = 2.0f;
};

#endif
