#include "tank.h"

#include <base/math.h>

#include <game/server/entities/character.h>
#include <game/server/gamecontext.h>

const char *CBlockClassTank::CommandName() const
{
	return "tank";
}

const char *CBlockClassTank::DisplayName() const
{
	return "Tank";
}

const char *CBlockClassTank::Description() const
{
	return "Lento, dificil de mover e resistente a impactos.";
}

void CBlockClassTank::OnCharacterSpawn(CGameContext *pGameServer, CCharacter *pCharacter)
{
	(void)pGameServer;
	if(!pCharacter)
	{
		return;
	}

	// Grant additional survivability right on spawn.
	pCharacter->IncreaseArmor(m_ArmorBoost);
}

void CBlockClassTank::OnCharacterTick(CGameContext *pGameServer, CCharacter *pCharacter)
{
	(void)pGameServer;
	if(!pCharacter)
	{
		return;
	}

	CCharacterCore Core = pCharacter->GetCore();

	const float GroundSpeed = (float)Core.m_Tuning.m_GroundControlSpeed;
	Core.m_Tuning.m_GroundControlSpeed = GroundSpeed * m_GroundSpeedMultiplier;

	const float GroundAccel = (float)Core.m_Tuning.m_GroundControlAccel;
	Core.m_Tuning.m_GroundControlAccel = GroundAccel * m_GroundAccelMultiplier;

	const float AirSpeed = (float)Core.m_Tuning.m_AirControlSpeed;
	Core.m_Tuning.m_AirControlSpeed = AirSpeed * m_AirSpeedMultiplier;

	const float AirAccel = (float)Core.m_Tuning.m_AirControlAccel;
	Core.m_Tuning.m_AirControlAccel = AirAccel * m_AirAccelMultiplier;

	const float HookAccel = (float)Core.m_Tuning.m_HookDragAccel;
	Core.m_Tuning.m_HookDragAccel = HookAccel * m_HookDragAccelMultiplier;

	const float HookSpeed = (float)Core.m_Tuning.m_HookDragSpeed;
	Core.m_Tuning.m_HookDragSpeed = HookSpeed * m_HookDragSpeedMultiplier;

	const float ShotgunStrength = (float)Core.m_Tuning.m_ShotgunStrength;
	Core.m_Tuning.m_ShotgunStrength = ShotgunStrength * m_ShotgunStrengthMultiplier;

	const float ExplosionStrength = (float)Core.m_Tuning.m_ExplosionStrength;
	Core.m_Tuning.m_ExplosionStrength = ExplosionStrength * m_ExplosionStrengthMultiplier;

	const float Speed = length(Core.m_Vel);
	if(Speed > m_MaxVelocity && Speed > 0.0001f)
	{
		Core.m_Vel *= m_MaxVelocity / Speed;
	}

	pCharacter->SetCore(Core);
}

void CBlockClassTank::OnCharacterAddVelocity(CGameContext *pGameServer, CCharacter *pCharacter, vec2 &Addition)
{
	(void)pGameServer;
	(void)pCharacter;
	Addition *= m_VelocityAdditionMultiplier;
}

void CBlockClassTank::OnCharacterTakeDamage(CGameContext *pGameServer, CCharacter *pCharacter, vec2 &Force, int &Damage, int From, int Weapon)
{
	(void)pGameServer;
	(void)pCharacter;
	(void)From;
	(void)Weapon;

	Force *= m_KnockbackMultiplier;
	if(Damage > 0)
	{
		int Reduced = static_cast<int>(Damage * m_DamageTakenMultiplier);
		if(Reduced <= 0)
		{
			Reduced = 1;
		}
		Damage = maximum(1, Reduced);
	}
}

void CBlockClassTank::OnCharacterPostCoreTick(CGameContext *pGameServer, CCharacter *pCharacter)
{
	(void)pGameServer;
	if(!pCharacter)
	{
		return;
	}

	const CCharacterCore *pCore = pCharacter->Core();
	if(!pCore)
	{
		return;
	}

	if(pCore->m_AttachedPlayers.empty())
	{
		return;
	}

	CCharacterCore Core = pCharacter->GetCore();
	Core.m_Vel *= m_HookedVelocityMultiplier;
	pCharacter->SetCore(Core);
}
