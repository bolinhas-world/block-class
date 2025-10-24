/* Block Class System - Soldier Class */
#ifndef GAME_SERVER_BLOCK_CLASS_SOLDIER_H
#define GAME_SERVER_BLOCK_CLASS_SOLDIER_H

#include "IPlayerClass.h"

/*
	SOLDIER CLASS
	
	Características:
	- Tanque de guerra com alta resistência
	- Vida e armadura aumentadas
	- Velocidade reduzida (mais pesado)
	- Dano aumentado com armas
	- Habilidade especial: Escudo temporário que absorve dano
*/

class CSoldierClass : public IPlayerClass
{
public:
	CSoldierClass() : IPlayerClass() {}
	~CSoldierClass() override {}

	// Informações da classe
	EPlayerClass GetClassType() const override { return CLASS_SOLDIER; }
	const char* GetClassName() const override { return "Soldier"; }
	const char* GetDescription() const override 
	{ 
		return "Tank class with high health and armor. Slower but deals more damage."; 
	}

	// Lifecycle hooks
	void OnSpawn(CCharacter* pCharacter) override;
	void OnDeath(CCharacter* pCharacter, int Killer, int Weapon) override;
	void OnTick(CCharacter* pCharacter) override;
	void OnWeaponFire(CCharacter* pCharacter, int Weapon) override;
	void OnPlayerDamage(CCharacter* pCharacter, vec2 Force, int Dmg, int From, int Weapon) override;

	// Stats base da classe
	int GetMaxHealth() const override { return 15; }  // +5 do padrão
	int GetMaxArmor() const override { return 15; }   // +5 do padrão
	float GetSpeedMultiplier() const override { return 0.85f; }  // 15% mais lento
	float GetDamageMultiplier() const override { return 1.25f; }  // 25% mais dano
	int GetHookLengthBonus() const override { return 0; }  // Sem bônus

	// Habilidades especiais
	int GetSpecialCooldown() const override { return 50 * 15; }  // 15 segundos
	void UseSpecialAbility(CCharacter* pCharacter) override;

private:
	bool m_ShieldActive;
	int m_ShieldEndTick;
	
	static const int SHIELD_DURATION = 50 * 5;  // 5 segundos
	static const int SHIELD_ABSORPTION = 75;  // Absorve 75% do dano
};

#endif // GAME_SERVER_BLOCK_CLASS_SOLDIER_H
