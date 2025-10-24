#ifndef GAME_SERVER_BLOCK_CLASS_BLOCK_CLASS_H
#define GAME_SERVER_BLOCK_CLASS_BLOCK_CLASS_H

#include <base/vmath.h>

class CCharacter;
class CGameContext;
class CPlayer;

class IBlockClass
{
public:
	virtual ~IBlockClass() = default;

	virtual const char *CommandName() const = 0;
	virtual const char *DisplayName() const = 0;
	virtual const char *Description() const = 0;

	virtual void OnAssigned(CGameContext *pGameServer, CPlayer *pPlayer);
	virtual void OnUnassigned(CGameContext *pGameServer, CPlayer *pPlayer);
	virtual void OnCharacterSpawn(CGameContext *pGameServer, CCharacter *pCharacter);
	virtual void OnCharacterTick(CGameContext *pGameServer, CCharacter *pCharacter);
	virtual void OnExplosionHit(CGameContext *pGameServer, int Weapon, CCharacter *pOwner, CCharacter *pTarget, vec2 HitPos, vec2 ForceDir);
};

#endif
