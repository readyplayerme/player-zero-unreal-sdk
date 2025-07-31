# 📦 Player Zero SDK for Unreal Engine

The Player Zero SDK makes it easy to load and display Ready Player Me avatars in Unreal Engine. It supports avatar loading, shortcodes, and render capture.

---

## ✅ Requirements

- Unreal Engine **5.0 or later**
- **Visual Studio 2022**
- [glTFRuntime Plugin](https://github.com/rdeioris/glTFRuntime)

---

## 🚀 Installation Guide
1. **Download the SDK**  
   Get the latest release from the [Releases Page](https://github.com/readyplayerme/player-zero-unreal-sdk/releases).
2. **Add to Plugins Folder**  
   Extract the contents of the ZIP file into your project’s `Plugins` directory: `YourProject/Plugins/PlayerZeroSdk/`
3. **Install glTFRuntime**
- Clone or download the [glTFRuntime plugin](https://github.com/rdeioris/glTFRuntime).
- Place it inside your `Plugins` folder alongside the PlayerZero SDK:
  `YourProject/Plugins/glTFRuntime/`
4. **Build the Project**  
   Open your project’s `.uproject` file. When prompted, choose to build the project in Visual Studio 2022.

    Alternatively, open the `.sln` file in Visual Studio and build the entire solution manually.

5. **Enable the Plugin**
- Launch the Unreal Editor.
- Go to **Edit > Plugins**
- Search for **Player Zero SDK** and enable it.
- Restart the editor when prompted.

---

## 🧪 Next Steps

- To verify installation, open the included **demo map**:
  `Content/PlayerZeroSdk/Maps/PlayerZeroDemo.umap`
- Check the documentation below to learn how to load avatars and avatar renders.


## 🎮 Demo Map: Testing the PlayerZero SDK

The plugin includes a sample map that demonstrates the full PlayerZero avatar loading and image rendering pipeline.

---

### 📍 Location

You can find the demo map inside the Player Zero plugin folder under: `Content/PlayerZeroDemo.umap`.

---

### 🧪 How to Open the Demo

1. In the **Content Browser**, navigate to:  
   `Content > PlayerZeroSdk > Maps`

2. Double-click on `PlayerZeroDemo` to open the map.

---

### 🎭 Map Setup

The map contains:

#### 🧱 `BP_PlayerZeroDemoActor`
- Found in the **World Outliner**
- Includes a `PlayerZeroLoaderComponent` which handles avatar loading
- In the Details panel, you can set:
    - `AvatarId`: Will auto-load the avatar on **BeginPlay**
    - `TargetSkeleton`: Optional skeleton for retargeting
    - `AnimationBlueprint`: Optional animation controller

---

### 🖼️ UI Widget: `WBP_PlayerZeroDemo`

- Automatically added to viewport in the **Level Blueprint**
- Contains:
    - `BP_AvatarCode` widget with a **text input field**
        - Input a shortcode and press Enter to fetch the corresponding avatar
    - **Avatar Image Preview**
        - Displays an avatar thumbnail loaded using the `LoadAvatarImageAsync` Blueprint node

---

### ✅ Testing the Demo

1. Press **Play** in the editor.
2. If `AvatarId` was set in `BP_PlayerZeroDemoActor`, an avatar should appear.
3. Test shortcode input:
    - Enter a valid **Avatar Code** into the input field
    - If successful, the avatar ID will be fetched and the avatar image will appear

---

### 💡 Pro Tip

You can extend the demo setup or use the `PlayerZeroLoaderComponent` and Blueprint nodes (`LoadAvatarImageAsync`) in your own UI or gameplay logic.

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