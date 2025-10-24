/* Block Class System - Player Class Interface */
#ifndef GAME_SERVER_BLOCK_CLASS_IPLAYERCLASS_H
#define GAME_SERVER_BLOCK_CLASS_IPLAYERCLASS_H

#include <base/vmath.h>

class CCharacter;
class CPlayer;

enum EPlayerClass
{
	CLASS_NONE = -1,
	CLASS_SOLDIER = 0,
	CLASS_NINJA,
	NUM_CLASSES
};

// Interface abstrata para todas as classes de jogadores
class IPlayerClass
{
public:
	virtual ~IPlayerClass() {}

	// Informações da classe
	virtual EPlayerClass GetClassType() const = 0;
	virtual const char* GetClassName() const = 0;
	virtual const char* GetDescription() const = 0;

	// Lifecycle hooks - chamados pelo servidor
	virtual void OnSpawn(CCharacter* pCharacter) = 0;
	virtual void OnDeath(CCharacter* pCharacter, int Killer, int Weapon) = 0;
	virtual void OnTick(CCharacter* pCharacter) = 0;
	virtual void OnWeaponFire(CCharacter* pCharacter, int Weapon) = 0;
	virtual void OnPlayerDamage(CCharacter* pCharacter, vec2 Force, int Dmg, int From, int Weapon) = 0;

	// Stats base da classe
	virtual int GetMaxHealth() const = 0;
	virtual int GetMaxArmor() const = 0;
	virtual float GetSpeedMultiplier() const = 0;
	virtual float GetDamageMultiplier() const = 0;
	virtual int GetHookLengthBonus() const = 0;  // Em tiles
	
	// Habilidades especiais
	virtual bool HasDoubleJump() const { return false; }
	virtual bool HasDash() const { return false; }
	virtual int GetSpecialCooldown() const { return 0; } // Em ticks
	virtual void UseSpecialAbility(CCharacter* pCharacter) {}
	
protected:
	// Cooldown management
	int m_LastSpecialUse;
	
	IPlayerClass() : m_LastSpecialUse(0) {}
	bool CanUseSpecial(int CurrentTick) const 
	{
		return (CurrentTick - m_LastSpecialUse) >= GetSpecialCooldown();
	}
	void SetSpecialUsed(int CurrentTick) 
	{
		m_LastSpecialUse = CurrentTick;
	}
};

#endif // GAME_SERVER_BLOCK_CLASS_IPLAYERCLASS_H
