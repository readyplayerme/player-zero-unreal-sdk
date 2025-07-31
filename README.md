# Ready Player Me NextGen SDK for Unreal Engine

This is the Unreal Engine SDK for Ready Player Me NextGen avatars. It allows you to easily integrate our avatars into your Unreal Engine project.

### Requirements
- Unreal Engine 5.0 or later
- Visual Studio 2022
- glTFRuntime plugin available here: https://github.com/rdeioris/glTFRuntime
- TransientObjectSaver plugin available here: https://github.com/readyplayerme/TransientObjectSaver

### Installation
1. Download the latest release from the [Releases](https://github.com/readyplayerme/rpm-unreal-next-gen-sdk/releases) page.
2. Extract the contents of the zip file into your project's Plugins directory. If the Plugins directory does not exist, create it.
3. Open your project's .sln file in your IDE and build.
4. Run your project and launch the Unreal Editor.
5. In the Editor, go to Edit -> Plugins and enable the Ready Player Me NextGen SDK plugin.

---
# Documentation
## 🎮 PlayerZeroLoaderComponent

The PlayerZeroLoaderComponent is a self-contained component that allows you to load a Ready Player Me avatar using an AvatarId and automatically attach it to your actor at runtime.
🚀 Features:
- Automatically loads a skeletal mesh from a glTF avatar asset
- Attaches the avatar to your actor using a SkeletalMeshComponent
- Supports assigning an animation blueprint
- Compatible with custom skeletons for advanced retargeting setups
- Blueprint and C++ compatible

---

## 🧩 Using PlayerZeroLoaderComponent (Blueprints)

The PlayerZeroLoaderComponent lets you load a Ready Player Me avatar in Blueprints with minimal setup. Add it to your actor, configure a few properties, and call LoadAvatar().
✅ 1. Add the Component

- Open your Blueprint actor (e.g. BP_Character, BP_Viewer).
- Click Add Component → search for PlayerZeroLoaderComponent.
- Name it something like AvatarLoader.

---
🧷 2. Set Up Required Properties

In the Details Panel of the AvatarLoader:

| Property              | Description                                                                 |
|-----------------------|-----------------------------------------------------------------------------|
| `Avatar Id`           | The Ready Player Me avatar ID (**required**).                               |
| `Animation Blueprint` | *(Optional)* Assign an animation blueprint class to drive the avatar.       |
| `Target Skeleton`     | *(Optional)* If using retargeting, assign your target skeleton here.         |
| `Target Mesh Component` | *(Optional)* Mesh to replace. If left empty, one will be auto-detected.     |

--- 

▶️ 3. Load the Avatar

Call Load Avatar in your Blueprint, for example:
Example: In BeginPlay

**Event BeginPlay** → **Load Avatar**

Or tie it to a UI Button press, login event, etc.

---

📣 4. Respond to Load Completion

Bind to the On Avatar Load Complete event in the component to detect when the avatar has finished loading.

AvatarLoader → On Avatar Load Complete (Event)
→ Do something with the loaded mesh (e.g., play animation, show UI, etc.)

The event passes the loaded SkeletalMeshComponent as a parameter.
💡 Tips
- If Target Skeleton is set, the avatar mesh will be validated against it (you’ll see warnings if bones don’t match).
- If your actor has no SkeletalMeshComponent, one will be created and attached automatically.
- This component is non-ticking and lightweight.

---

🧪 Sample Use Case

Want to let users customize their avatar via web and launch your game? Simply:

- Pass the AvatarId to your game (e.g., via Steam deep link, or UI input).
- Set it on the component.
- Call Load Avatar.

Done!