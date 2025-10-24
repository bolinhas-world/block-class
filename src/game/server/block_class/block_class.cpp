#include "block_class.h"

#include <game/server/entities/character.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>

void IBlockClass::OnAssigned(CGameContext *pGameServer, CPlayer *pPlayer)
{
	(void)pGameServer;
	(void)pPlayer;
}

void IBlockClass::OnUnassigned(CGameContext *pGameServer, CPlayer *pPlayer)
{
	(void)pGameServer;
	(void)pPlayer;
}

void IBlockClass::OnCharacterSpawn(CGameContext *pGameServer, CCharacter *pCharacter)
{
	(void)pGameServer;
	(void)pCharacter;
}

void IBlockClass::OnCharacterTick(CGameContext *pGameServer, CCharacter *pCharacter)
{
	(void)pGameServer;
	(void)pCharacter;
}

float IBlockClass::AdjustWeaponFireDelay(CGameContext *pGameServer, CCharacter *pCharacter, int Weapon, float BaseDelay) const
{
	(void)pGameServer;
	(void)pCharacter;
	(void)Weapon;
	return BaseDelay;
}

void IBlockClass::OnProjectileHit(CGameContext *pGameServer, int Weapon, CCharacter *pOwner, CCharacter *pTarget, vec2 HitPos)
{
	(void)pGameServer;
	(void)Weapon;
	(void)pOwner;
	(void)pTarget;
	(void)HitPos;
}

void IBlockClass::OnExplosionHit(CGameContext *pGameServer, int Weapon, CCharacter *pOwner, CCharacter *pTarget, vec2 HitPos, vec2 ForceDir)
{
	(void)pGameServer;
	(void)Weapon;
	(void)pOwner;
	(void)pTarget;
	(void)HitPos;
	(void)ForceDir;
}
