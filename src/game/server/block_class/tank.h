#ifndef GAME_SERVER_BLOCK_CLASS_TANK_H
#define GAME_SERVER_BLOCK_CLASS_TANK_H

#include "block_class.h"

class CBlockClassTank final : public IBlockClass
{
public:
	const char *CommandName() const override;
	const char *DisplayName() const override;
	const char *Description() const override;

	void OnCharacterSpawn(CGameContext *pGameServer, CCharacter *pCharacter) override;
	void OnCharacterTick(CGameContext *pGameServer, CCharacter *pCharacter) override;
	void OnCharacterAddVelocity(CGameContext *pGameServer, CCharacter *pCharacter, vec2 &Addition) override;
	void OnCharacterTakeDamage(CGameContext *pGameServer, CCharacter *pCharacter, vec2 &Force, int &Damage, int From, int Weapon) override;
	void OnCharacterPostCoreTick(CGameContext *pGameServer, CCharacter *pCharacter) override;

private:
	float m_GroundSpeedMultiplier = 0.75f;
	float m_GroundAccelMultiplier = 0.7f;
	float m_AirSpeedMultiplier = 0.8f;
	float m_AirAccelMultiplier = 0.7f;
	float m_HookDragAccelMultiplier = 0.6f;
	float m_HookDragSpeedMultiplier = 0.6f;
	float m_ShotgunStrengthMultiplier = 0.7f;
	float m_ExplosionStrengthMultiplier = 0.7f;
	float m_VelocityAdditionMultiplier = 0.5f;
	float m_KnockbackMultiplier = 0.4f;
	float m_DamageTakenMultiplier = 0.7f;
	float m_MaxVelocity = 12.0f;
	float m_HookedVelocityMultiplier = 0.55f;
	int m_ArmorBoost = 10;
};

#endif
