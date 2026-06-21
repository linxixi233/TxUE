# FPS Multiplayer 5.8 — 项目报告

> **课程大作业**：基于 UE5 官方 First Person 模板，实现一个多人 FPS 游戏 Demo
>
> **引擎版本**：Unreal Engine 4.3（前两日） 5.3、5.8（后一日）
>
> **开发周期**：6，19——6，21

---

## 一、个人学习背景与感悟

### 1.1 首次接触成熟商业引擎

在本次项目之前，我从未使用过 Unity、Unreal Engine、Godot 这类成熟的商业/开源游戏引擎。过去我编写和使用的是自研小引擎——自己搭渲染管线、手写碰撞检测、从零构建游戏循环的那种。这种经历让我对引擎底层原理有一定理解，但也让我对"商业引擎到底做了什么"缺乏直观认知。

这次 UE5 的学习之旅，对我而言是一次彻底的认知刷新。

### 1.2 初见蓝图：惊艳与错觉

UE5 的**蓝图可视化脚本系统**（Blueprint Visual Scripting）给我的第一印象是"这也太简单了"。拖拽节点、连线、设置属性——几乎所有功能都是开箱即用的。引擎内置的节点库覆盖了从角色移动、碰撞检测、动画驱动、音效播放到网络复制的方方面面。

上手仅数日，我便能理解以下概念：

- **资产绑定**：骨骼网格体（Skeletal Mesh）与动画蓝图（Animation Blueprint）之间的关联，通过 `AnimClass` 属性指定即可完成角色与动画的绑定
- **骨骼迁移管线**：不同骨架（Skeleton）之间的动画共享——例如将 `SK_Mannequin` 的动画序列（Quinn 动画包）通过**动画混合空间**（Blend Space）驱动敌人 AI 角色的空闲/行走/奔跑状态切换，理解了 UE 的动画重定向（Retargeting）机制

这种"几天就能上手"的感觉给了我一个错觉：**用 UE5 做游戏应该很快、很轻松。**

### 1.3 实践后才发现：难度并不小

真正沉下来做这个多人 FPS Demo 时，困难才开始显现：

1. **蓝图并非万能**：虽然蓝图节点极其丰富，但当需要实现复杂的多人网络逻辑、自定义武器系统架构时，仅靠拖拽节点是不够的——需要理解 UE 的对象模型（`Actor` → `Pawn` → `Character`）、组件系统（`ActorComponent` → `SceneComponent` → `SkeletalMeshComponent`）、以及 `GameMode` / `GameState` / `PlayerController` / `PlayerState` 等框架类的职责划分。

2. **多人网络是硬骨头**：从单机到多人，需要理解 UE 的**复制（Replication）机制**——哪些变量需要复制、哪些函数需要标记为 `Server` / `Client` / `NetMulticast`、`GameMode` 仅在服务端存在而 `PlayerController` 每个客户端一个……这些概念对初学者而言相当陡峭。

3. **架构设计需要经验**：如何设计武器系统的继承层级？拾取物应该用接口还是基类？金币计数的数据应该放在 `Character` 还是 `GameState` 上？这些问题没有唯一的正确答案，但每种选择都有不同的后果。成熟引擎的强大之处在于提供了"正确的默认路径"，但要真正理解这些默认路径为何正确，需要大量实践。

4. **资产管线不透明**：虽然引擎的导入器很强大，但 `.uasset` / `.umap` 是二进制文件，无法像纯代码项目那样用 Git diff 查看变更。资产之间的引用关系（软引用 / 硬引用）一旦出错，排查起来非常困难。

总的来说，UE5 降低了游戏开发"能做到"的门槛，但"做好"的门槛依然很高——而两者之间的差距，恰恰是成熟引擎最值得深入学习的部分。

---

## 二、作业要求与完成情况

### 2.1 原始需求

| #  | 要求                                      | 说明                                     |
|----|-------------------------------------------|------------------------------------------|
| 1  | 敌人移动与攻击玩家，玩家可击败敌人        | AI 行为 + 伤害/死亡系统                  |
| 2  | 基础得分与游戏胜利机制                    | 玩法自由发挥                             |
| 3  | 多人网络对战                              | 联机大厅 + 会话管理                      |
| 4  | （选做）渲染、AI 等进阶内容               | 有余力自行扩展                           |

### 2.2 完成情况总览

| 功能模块                | 状态      | 说明                                                       |
|-------------------------|-----------|------------------------------------------------------------|
| ✅ 敌人移动跟随          | **已完成** | `BP_EnemyAi` + `PawnSensing` + 寻路                          |
| ✅ 敌人攻击玩家          | **已完成** | 动画蒙太奇 + `canAttack` 状态机                              |
| ✅ 多人网络对战大厅      | **已完成** | 主菜单 → 服务器浏览器 → 服务器面板 UI 框架           |
| ✅ 基础金币得分机制      | **已完成** | `BP_Coin` + `WBP_Coin` HUD + 收集反馈                       |
| ✅ 自研武器系统          | **已完成** | 主/副武器槽位、开火模式枚举、多武器支持、拾取交互接口        |
| ⬜重写第一人称角色类    | **未全部完成** | 基于模板重写，集成自定义武器与交互系统，编写有主副手武器接口                        |
| ⬜ 游戏胜利机制          | **待完成** | 需添加关卡金币全部收集后触发胜利条件                        |
| ⬜ 玩家受伤机制          | **待完成** | 需在BP_FirstPersonCharacter中加入变量与BP_Enemy的碰撞伤害检测、以及关卡界面UI层的血量显示，记分板显示                        |

---

## 三、项目技术架构

### 3.1 目录结构

```
fps_multiplayer 5.8/
├── fps_multiplayer.uproject          # 项目定义 (UE 5.8)
├── Config/
│   ├── DefaultEngine.ini             # 引擎配置 (地图、渲染、碰撞)
│   ├── DefaultInput.ini              # 增强输入系统映射
│   ├── DefaultGame.ini               # 游戏设置
│   └── DefaultEditor.ini             # 编辑器偏好
├── Source/
│   └── fps_multiplayer/Game/
│       └── MenuGameMode.cpp          # 主菜单 C++ GameMode
├── Content/
│   ├── FirstPerson/                  # UE 官方第一人称模板
│   │   ├── Blueprints/               # BP_FirstPersonCharacter, BP_Weapon_Component
│   │   ├── Input/                    # IA_Shoot, IA_Interact, IMC_Weapons
│   │   └── Maps/                     # FirstPersonMap (游玩关卡)
│   ├── _Main/                        # ✨ 全部自定义内容
│   │   ├── props/
│   │   │   ├── Weapons/              # BP_Weapon_Comp_Matser, BP_AssaultRifle
│   │   │   │                        # Enum_FireMode, Enum_WeaponClass, Enum_WeaponName
│   │   │   └── Pickups/              # BP_PickupMaster, BP_WeaponPickupMaster, BP_AR_B_Pickup
│   │   ├── Enemy_Blueprint/          # BP_EnemyAi, AP_EnemyAi, Walk_Space1D
│   │   ├── coin/                     # BP_Coin, Coin1 (3D模型), Coin.fbx
│   │   ├── Menu_Blueprint/           # WBP_Menu, WBP_Servers, WBP_ServerPanel
│   │   ├── UI_Player/                # WBP_Coin (HUD)
│   │   ├── interfaces/               # BPI_Interact (交互接口)
│   │   └── LVL_MainMenu.umap         # 主菜单关卡
│   ├── FPS_Weapon_Bundle/            # 军事武器资产包 (AR4, AK47, SMG11...)
│   ├── MilitaryWeapDark/             # 深色涂装武器包
│   ├── MilitaryWeapSilver/           # 银色涂装武器包
│   ├── Characters/                   # SK_Mannequin 骨骼网格
│   └── AnimStarterPack/              # 入门动画包
```

### 3.2 游戏模式架构

```
[主菜单]  LVL_MainMenu
    │       AMenuGameMode (C++)
    │       WBP_Menu → WBP_Servers → WBP_ServerPanel
    │
    └──────→ [游戏关卡] FirstPersonMap
                BP_FirstPersonGameMode (蓝图)
                ├─ BP_FirstPersonCharacter (玩家)
                │   ├─ BP_Weapon_Comp_Matser (武器系统)
                │   └─ BPI_Interact (交互拾取)
                ├─ BP_EnemyAi × N (敌人)
                │   └─ AP_EnemyAi (动画)
                └─ BP_Coin × M (收集品)
```

---

## 四、各系统技术实现详解

### 4.1 重写武器系统

**设计理念**：完全抛弃 UE 模板中 `BP_Weapon_Component` 的单武器方案，构建可扩展的武器框架。

**继承层级**：

```
ActorComponent
  └── BP_Weapon_Comp_Matser          ← 自定义武器组件基类
        ├── Damage (float)           ← 伤害值
        ├── Range (float)            ← 射程
        ├── FireMode (Enum)          ← Semi Auto / Full Auto
        ├── WeaponName (Enum)        ← Unarmed / Pistol / AssaultRifle
        ├── PrimaryWeapon (Object)   ← 主武器槽
        ├── SecondaryWeapon (Object) ← 副武器槽
        ├── SocketName (Name)        ← 手部挂载点
        └── HolsterSocketName (Name) ← 收起挂载点

            └── BP_AssaultRifle      ← 具体突击步枪
                  ├── FireMode = Full Auto
                  ├── SkeletalMesh + 开火蒙太奇
                  └── PickupClass → BP_AR_B_Pickup

Actor (BP_PickupMaster)              ← 拾取物基类
  ├── BPI_Interact 接口实现
  ├── PickupRadius
  └── Interact() / GetWeaponClass()

      └── BP_WeaponPickupMaster       ← 武器拾取
            ├── WeaponToSpawn (ClassRef)
            └── PickupWeaponClass (Enum)

            └── BP_AR_B_Pickup        ← 突击步枪拾取
                  └── WeaponToSpawn = BP_AssaultRifle_C
```

**关键枚举**：

| 枚举                | 值                              |
|---------------------|---------------------------------|
| `Enum_FireMode`     | Semi Auto / Full Auto           |
| `Enum_WeaponClass`  | Primary / Secondary             |
| `Enum_WeaponName`   | Unarmed / Pistol / AssaultRifle |

**交互流程**：

```
玩家靠近武器 → 按交互键 (IA_Interact)
  → BPI_Interact::GetWeaponClass()
    → 判断主/副武器槽
      → BPI_Interact::Interact(Character, WeaponClass)
        → AddComponentByClass 动态添加武器组件
        → K2_AttachToComponent 挂载到角色 AR_B_Socket
```

**武器资产规模**：11 种武器 + 6 类配件 + 9 种弹药类型，涵盖深色/银色双涂装体系。

---

### 4.2 多人联机大厅

**菜单 UI 架构**：

```
AMenuGameMode (C++ GameMode)
  └── BeginPlay() → ShowMenu()
        └── CreateWidget<UW_MainMenu>(MainMenuWidgetClass)
              └── AddToViewport(0)

WBP_Menu (主菜单界面)
  ├── [开始游戏] → OpenLevel(FirstPersonMap)
  ├── [多人游戏] → 切换到 WBP_Servers
  └── [退出游戏] → QuitGame()

WBP_Servers (服务器浏览器)
  └── ScrollBox
        ├── WBP_ServerPanel (服务器 1)
        │     ├── 服务器名称 (TextBlock)
        │     ├── 玩家数 3/16 (TextBlock)
        │     └── 延迟 23ms (TextBlock)
        ├── WBP_ServerPanel (服务器 2)
        └── ...
```

**当前状态**：UI 框架已完整搭建，`MenuGameMode.cpp` 负责关卡加载时自动弹出主菜单。实际的 `OnlineSubsystem` 会话发现 / 创建 / 加入逻辑预留了接入点，作为下一步开发目标。

**配置关联**：

```ini
; DefaultEngine.ini
GameDefaultMap=/Game/_Main/LVL_MainMenu.LVL_MainMenu
ServerDefaultMap=/Engine/Maps/Entry
GlobalDefaultGameMode=/Game/FirstPerson/Blueprints/BP_FirstPersonGameMode.BP_FirstPersonGameMode_C
```

---

### 4.3 金币收集小游戏

**完整数据流**：

```
┌─ BP_Coin (关卡片段中的 Actor) ─────────────────────┐
│  ├─ StaticMeshComponent → Coin1 3D网格体              │
│  ├─ SphereComponent → 重叠检测碰撞体                   │
│  ├─ RotatingMovementComponent → 持续自转吸引注意       │
│  ├─ TimelineComponent → 浮动动画曲线                   │
│  └─ AudioComponent → 收集反馈音效                      │
│                                                       │
│  OnComponentBeginOverlap (Sphere)                     │
│       │                                               │
│       ▼                                               │
│  if (OtherActor == BP_FirstPersonCharacter):           │
│    ├─ SpawnSoundAttached(Coins_08)                     │
│    ├─ BP_FirstPersonCharacter.AddCoin()   ←────────┐  │
│    └─ DestroyActor()                               │  │
└────────────────────────────────────────────────────┘  │
                                                       │
BP_FirstPersonCharacter                                │
  ├─ CoinCount (int)  ←──────────────────────────────┘
  └─ EatCoin 委托 ─────→ WBP_Coin (HUD Widget)
                              ├─ TextBlock
                              ├─ CoinCount → Conv_IntToText → SetText()
                              └─ ColorAndOpacity, Font
```

**技术细节**：

| 方面       | 实现                                                     |
|------------|----------------------------------------------------------|
| 3D 模型    | `Coin.fbx` 导入 → `Coin1.uasset`，含 AO 环境光遮蔽贴图    |
| 视觉反馈   | `RotatingMovementComponent` 自转 + `TimelineComponent` 浮动 |
| 收集判定   | `SphereComponent` 触发 `OnComponentBeginOverlap`           |
| 音频反馈   | `SpawnSoundAttached` 播放 15 种硬币音效变体               |
| HUD 更新   | `WBP_Coin` 在 `PreConstruct` 绑定角色 `EatCoin` 委托      |

---

### 4.4 敌人跟随 AI

**感知 → 寻路 → 动画 全链路**：

```
BP_EnemyAi (Character)
  ├─ CapsuleComponent              ← 物理碰撞
  ├─ SkeletalMeshComponent         ← SK_Mannequin 骨骼网格
  ├─ CharacterMovementComponent    ← 角色移动控制
  └─ PawnSensingComponent          ← 听觉/视觉感知

      │ OnSeePawn 委托触发
      ▼
  AI MoveTo (CreateMoveToProxyObject)
      │ 受 AcceptanceRadius 控制到达判定
      │
      ▼  (速度 → 动画系统)
  AP_EnemyAi (AnimInstance / 动画蓝图)
      │
      ├─ BlueprintUpdateAnimation (每帧)
      │    ├─ TryGetPawnOwner → GetCharacterMovement
      │    └─ GetVelocity → VSize() → Speed
      │
      └─ AnimGraph
            └─ Walk_Space1D (1D Blend Space)
                  ├─ Speed = 0.0  → MF_Idle      (待机)
                  ├─ Speed = 中值 → MF_Walk_Fwd  (行走)
                  └─ Speed = 最大值 → MF_Run_Fwd   (奔跑)
                       │
                       └─ Slot 节点 (可叠加攻击蒙太奇)
```

**技术细节**：

| 组件                     | 用途                                         |
|--------------------------|----------------------------------------------|
| `PawnSensingComponent`   | UE 原生视觉/听觉感知，检测到玩家时触发 `OnSeePawn`   |
| `AIMoveTo`               | 基于导航网格的寻路移动                         |
| `AcceptanceRadius`       | 到达目标判定半径                              |
| `Walk_Space1D`           | 基于 Speed 的 1D 动画混合空间                  |
| `canAttack`              | 攻击状态机布尔开关                            |
| `AnimMontage Slot`       | 动画蒙太奇叠加层（攻击动画覆盖在移动动画之上）     |

**骨架/动画资源**：

| 资源               | 路径/来源                                        |
|--------------------|--------------------------------------------------|
| 骨骼网格           | `SK_Mannequin`                                   |
| 待机动画           | `MF_Idle` (Quinn 动画包)                         |
| 行走动画           | `MF_Walk_Fwd`                                    |
| 奔跑动画           | `MF_Run_Fwd`                                     |
| 攻击蒙太奇         | `NewAnimMontage` (自定义)                         |

---


## 五、学习收获

1. **蓝图系统的强大与局限**：UE5 蓝图绝对是游戏原型开发的利器，节点库的覆盖度令人惊叹。但当项目规模增长时，蓝图的"视觉连线"管理成本会急剧上升，此时 C++ 的代码组织能力优势凸显。理解两者如何混合使用（C++ 定义基类和核心逻辑，蓝图做配置和快速迭代）是 UE 开发的关键技能。

2. **成熟引擎的框架约束**：与自研小引擎"一切自己说了算"不同，UE5 有非常明确的框架设计——`GameMode` 只在服务端、`PlayerController` 每个客户端一个、`GameState` 全局共享……这些规则不是限制，而是多年多人游戏开发经验沉淀下来的最佳实践。遵循它们，很多网络同步问题会自然得到解决。

3. **资产管线的深度**：从 FBX 导入、材质编辑、骨架重定向、动画混合空间到物理资产，UE5 的美术管线是一条完整的流水线。理解它需要的不只是"知道怎么点按钮"，而是理解每一步背后的美术和性能考量。

4. **"简单"与"正确"的差距**：在 UE5 里搭一个能跑的游戏原型确实很快——模板 + 几个蓝图节点就能让角色动起来。但要让系统可扩展（换一把武器不影响整个工程）、可多人同步（复制不出 BUG）、可维护（三个月后还能读懂自己的蓝图），需要的是架构思维——这与用什么引擎无关。

---

*项目时间：6-19——6—21，记录了首次使用 Unreal Engine 进行游戏开发的学习历程与项目成果。*
