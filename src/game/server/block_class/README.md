# Block Class System

Sistema de classes para DDNet que adiciona diferentes classes de personagens com habilidades únicas.

## Arquitetura

O sistema foi projetado com uma arquitetura modular e separada que se integra com o código principal do servidor através de chamadas específicas:

```
src/game/server/block_class/
├── classes.h              # CClassManager - Gerenciador principal
├── classes.cpp            # Implementação do gerenciador
└── classes/
    ├── IPlayerClass.h     # Interface abstrata base
    ├── Soldier.h          # Classe Soldier (header)
    ├── Soldier.cpp        # Classe Soldier (implementação)
    ├── Ninja.h            # Classe Ninja (header)
    └── Ninja.cpp          # Classe Ninja (implementação)
```

### Componentes Principais

#### 1. IPlayerClass (Interface Base)

- Define a interface abstrata para todas as classes
- Métodos de lifecycle: `OnSpawn()`, `OnDeath()`, `OnTick()`, `OnWeaponFire()`, `OnPlayerDamage()`
- Getters para stats: `GetMaxHealth()`, `GetMaxArmor()`, `GetSpeedMultiplier()`, etc.
- Sistema de habilidades especiais com cooldown

#### 2. CClassManager (Gerenciador)

- Gerencia todas as instâncias de classes dos jogadores
- Factory pattern para criar instâncias de classes
- Despacha eventos para as classes apropriadas
- Gerencia seleção de classes pelos jogadores

#### 3. Classes Concretas

##### Soldier

- **Papel**: Tank/Resistente
- **Stats**:
  - Vida: 15 (+5)
  - Armadura: 15 (+5)
  - Velocidade: 85% (-15%)
  - Dano: 125% (+25%)
- **Habilidade Especial**: Shield (Escudo)
  - Cooldown: 15 segundos
  - Duração: 5 segundos
  - Efeito: 75% de redução de dano

##### Ninja

- **Papel**: Ágil/Rápido
- **Stats**:
  - Vida: 7 (-3)
  - Armadura: 7 (-3)
  - Velocidade: 135% (+35%)
  - Dano: 100% (normal)
  - Gancho: +5 tiles
- **Habilidade Especial**: Dash
  - Cooldown: 8 segundos
  - Efeito: Teleporte rápido de 15 tiles na direção do mouse
- **Passiva**: Double Jump

## Integração com o Servidor

O sistema se integra com o código principal através de hooks estratégicos:

### 1. CGameContext

```cpp
// gamecontext.h
class CGameContext {
    CClassManager *m_pClassManager;
public:
    CClassManager *ClassManager() { return m_pClassManager; }
};

// gamecontext.cpp - Inicialização
m_pClassManager = new CClassManager(this);
m_pClassManager->Init();
```

### 2. CCharacter (Eventos)

```cpp
// OnSpawn
if(GameServer()->ClassManager())
    GameServer()->ClassManager()->OnPlayerSpawn(this);

// OnDeath
if(GameServer()->ClassManager())
    GameServer()->ClassManager()->OnPlayerDeath(this, Killer, Weapon);

// OnTick
if(GameServer()->ClassManager())
    GameServer()->ClassManager()->OnPlayerTick(this);

// OnWeaponFire
if(GameServer()->ClassManager())
    GameServer()->ClassManager()->OnWeaponFire(this, m_Core.m_ActiveWeapon);
```

### 3. Comandos de Chat

Implementados em `blockclasschat.cpp`:

- `/class [nome]` - Seleciona uma classe
- `/classinfo` - Mostra informações da classe atual
- `/special` - Usa a habilidade especial da classe

## Como Usar

### Para Jogadores

1. **Selecionar uma Classe**:

   ```
   /class soldier  # Seleciona a classe Soldier
   /class ninja    # Seleciona a classe Ninja
   /class          # Mostra todas as classes disponíveis
   ```

2. **Ver Informações da Classe**:

   ```
   /classinfo      # Mostra stats e habilidades da sua classe
   ```

3. **Usar Habilidade Especial**:
   ```
   /special        # Usa a habilidade especial (respeita cooldown)
   ```

### Para Desenvolvedores

#### Adicionar uma Nova Classe

1. **Criar o Header** (`classes/NovaClasse.h`):

```cpp
#include "IPlayerClass.h"

class CNovaClasse : public IPlayerClass
{
public:
    EPlayerClass GetClassType() const override { return CLASS_NOVA; }
    const char* GetClassName() const override { return "Nova Classe"; }

    // Implementar métodos virtuais
    void OnSpawn(CCharacter* pCharacter) override;
    // ... outros métodos
};
```

2. **Implementar** (`classes/NovaClasse.cpp`):

```cpp
#include "NovaClasse.h"

void CNovaClasse::OnSpawn(CCharacter* pCharacter)
{
    // Lógica de spawn
}
```

3. **Adicionar ao Enum** (`classes/IPlayerClass.h`):

```cpp
enum EPlayerClass
{
    CLASS_NONE = -1,
    CLASS_SOLDIER = 0,
    CLASS_NINJA,
    CLASS_NOVA,     // Adicionar aqui
    NUM_CLASSES
};
```

4. **Atualizar Factory** (`classes.cpp`):

```cpp
IPlayerClass* CClassManager::CreateClassInstance(EPlayerClass ClassType)
{
    switch(ClassType)
    {
        case CLASS_SOLDIER: return new CSoldierClass();
        case CLASS_NINJA: return new CNinjaClass();
        case CLASS_NOVA: return new CNovaClasse();  // Adicionar
        default: return nullptr;
    }
}
```

5. **Atualizar Comando** (`blockclasschat.cpp`):

```cpp
if(str_comp_nocase(pClassName, "nova") == 0)
    ClassType = CLASS_NOVA;
```

## Benefícios da Arquitetura

1. **Separação de Responsabilidades**: O sistema de classes está completamente isolado em sua própria pasta
2. **Fácil Manutenção**: Mudanças no sistema de classes não afetam o resto do código
3. **Extensibilidade**: Adicionar novas classes é simples e segue um padrão claro
4. **Reutilização**: A interface IPlayerClass pode ser facilmente estendida
5. **Testabilidade**: Componentes isolados são mais fáceis de testar

## Notas Técnicas

- O ClassManager é criado e destruído junto com o GameContext
- Cada jogador pode ter apenas uma classe por vez
- As classes são resetadas quando o jogador desconecta
- Os eventos são chamados apenas quando o ClassManager está inicializado
- O sistema usa o padrão Factory para criar instâncias de classes
- Cooldowns são gerenciados em ticks do servidor (50 ticks = 1 segundo)

## Possíveis Melhorias Futuras

1. Persistência de classes (salvar no banco de dados)
2. Sistema de experiência/level para classes
3. Mais classes (Medic, Engineer, Scout, etc.)
4. Habilidades passivas mais complexas
5. Sistema de skins por classe
6. Balanceamento dinâmico de stats
7. Modo de treino para testar classes
8. Estatísticas por classe
