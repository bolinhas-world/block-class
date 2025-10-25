#ifndef GAME_SERVER_BLOCK_CLASS_KANGARO_H
#define GAME_SERVER_BLOCK_CLASS_KANGARO_H

#include "block_class.h"

class CBlockClassKangaro final : public IBlockClass
{
public:
	const char *CommandName() const override;
	const char *DisplayName() const override;
	const char *Description() const override;

	float AdjustWeaponFireDelay(CGameContext *pGameServer, CCharacter *pCharacter, int Weapon, float BaseDelay) const override;
	bool OnCharacterGunFire(CGameContext *pGameServer, CCharacter *pCharacter, vec2 ProjStartPos, vec2 Direction, vec2 MouseTarget) override;

private:
	float m_GunFireDelayMs = 3000.0f;
};

#endif
