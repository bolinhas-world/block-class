/* Block Class System - Class Manager */
#ifndef GAME_SERVER_BLOCK_CLASS_MANAGER_H
#define GAME_SERVER_BLOCK_CLASS_MANAGER_H

#include "classes/IPlayerClass.h"
#include <base/system.h>
#include <engine/shared/protocol.h>

class CGameContext;
class CPlayer;
class CCharacter;

/*
	CClassManager
	
	Gerenciador central do sistema de classes.
	Responsável por:
	- Criar e destruir instâncias de classes
	- Atribuir classes aos jogadores
	- Despachar eventos para as classes apropriadas
	- Gerenciar seleção de classes
*/

class CClassManager
{
public:
	CClassManager(CGameContext* pGameServer);
	~CClassManager();

	// Inicialização
	void Init();
	void Reset();

	// Gerenciamento de classes por jogador
	bool SetPlayerClass(int ClientId, EPlayerClass ClassType);
	EPlayerClass GetPlayerClass(int ClientId) const;
	IPlayerClass* GetPlayerClassInstance(int ClientId) const;
	const char* GetPlayerClassName(int ClientId) const;

	// Eventos - chamados pelo servidor
	void OnPlayerSpawn(CCharacter* pCharacter);
	void OnPlayerDeath(CCharacter* pCharacter, int Killer, int Weapon);
	void OnPlayerTick(CCharacter* pCharacter);
	void OnWeaponFire(CCharacter* pCharacter, int Weapon);
	void OnPlayerDamage(CCharacter* pCharacter, vec2 Force, int Dmg, int From, int Weapon);
	
	// Habilidades especiais
	void UsePlayerSpecialAbility(int ClientId);
	
	// Seleção de classe
	void OpenClassSelectionMenu(int ClientId);
	void HandleClassSelection(int ClientId, int ClassIndex);

	// Utilidades
	int GetClassCount() const { return NUM_CLASSES; }
	const char* GetClassNameByType(EPlayerClass ClassType) const;
	const char* GetClassDescriptionByType(EPlayerClass ClassType) const;

private:
	CGameContext* m_pGameServer;
	
	// Instâncias de classes por jogador
	IPlayerClass* m_aPlayerClasses[MAX_CLIENTS];
	EPlayerClass m_aPlayerClassTypes[MAX_CLIENTS];
	
	// Factory methods
	IPlayerClass* CreateClassInstance(EPlayerClass ClassType);
	void DestroyClassInstance(int ClientId);
	
	// Validação
	bool IsValidClientId(int ClientId) const;
	bool IsValidClassType(EPlayerClass ClassType) const;
};

#endif // GAME_SERVER_BLOCK_CLASS_MANAGER_H
