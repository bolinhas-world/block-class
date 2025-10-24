#ifndef GAME_SERVER_BLOCK_CLASS_NINJA_H
#define GAME_SERVER_BLOCK_CLASS_NINJA_H

#include "block_class.h"

class CBlockClassNinja final : public IBlockClass
{
public:
	const char *CommandName() const override;
	const char *DisplayName() const override;
	const char *Description() const override;

	void OnCharacterTick(CGameContext *pGameServer, CCharacter *pCharacter) override;

private:
	float m_GroundSpeedMultiplier = 1.8f;
};

#endif
