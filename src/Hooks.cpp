#include "Hooks.h"

namespace Hooks {

// ---------------------------------------------------------------------------
// PlayerHook::Install
// ---------------------------------------------------------------------------
void PlayerHook::Install() {
  REL::Relocation<std::uintptr_t> playerVtbl{RE::VTABLE_PlayerCharacter[0]};
  _Update = playerVtbl.write_vfunc(0xAD, Update);

  SKSE::log::info(
      "PlayerHook: Update hook installed. Waiting for player init...");
}

// ---------------------------------------------------------------------------
// NotifyAnimationGraph
// ---------------------------------------------------------------------------
bool PlayerHook::NotifyAnimationGraph(
    RE::IAnimationGraphManagerHolder *a_holder,
    const RE::BSFixedString &a_eventName) {

  if (a_holder) {
    auto actor = skyrim_cast<RE::Actor *>(a_holder);
    if (actor && actor->IsPlayerRef()) {
      const char *eventName = a_eventName.c_str();
      if (eventName) {

        // 1. Block Turn events completely
        if (strcmp(eventName, "TurnLeft") == 0 ||
            strcmp(eventName, "TurnRight") == 0) {
          return false;
        }

        // 2. Equip / Unequip logic
        if (stristr(eventName, "WeapEquip") || stristr(eventName, "Unequip")) {

          // Start equip phase
          if (stristr(eventName, "WeapEquip") && !stristr(eventName, "_Out")) {
            g_IsEquipping.store(true);
          } else if (stristr(eventName, "Unequip") &&
                     !stristr(eventName, "_Out")) {
            g_IsEquipping.store(true);
          }

          // End equip phase + 166ms buffer
          if (stristr(eventName, "WeapEquip_Out") ||
              stristr(eventName, "Unequip_Out")) {
            g_IsEquipping.store(false);
            g_EquipTimer.store(0.166f);  // ~166ms regardless of FPS
            SKSE::log::info(
                ">>> Equip Out Event. Starting 0.166s stop timer.");
          }
        }
      }
    }
  }

  // Safety guard
  if (_NotifyAnimationGraph_Orig == 0) {
    return true;
  }

  auto func = reinterpret_cast<bool (*)(RE::IAnimationGraphManagerHolder *,
                                        const RE::BSFixedString &)>(
      _NotifyAnimationGraph_Orig);
  return func(a_holder, a_eventName);
}

// ---------------------------------------------------------------------------
// Update (Every Frame)
// ---------------------------------------------------------------------------
void PlayerHook::Update(RE::Character *a_this, float a_delta) {
  if (a_this && a_this->IsPlayerRef() && a_this->Is3DLoaded()) {
    // 1. Install NotifyAnimationGraph hook once
    static std::atomic<bool> hooked{false};
    bool expected = false;
    if (hooked.compare_exchange_strong(expected, true)) {
      auto holder = static_cast<RE::IAnimationGraphManagerHolder *>(a_this);
      std::uintptr_t *holderVtbl = *reinterpret_cast<std::uintptr_t **>(holder);
      _NotifyAnimationGraph_Orig = holderVtbl[1];
      REL::safe_write(reinterpret_cast<std::uintptr_t>(&holderVtbl[1]),
                      reinterpret_cast<std::uintptr_t>(NotifyAnimationGraph));
      SKSE::log::info("PlayerHook: NotifyAnimationGraph hook installed.");
    }

    // 2. Movement state check — direct engine query, SE/AE compatible.
    const bool isMoving = a_this->IsMoving();

    static bool s_WasMoving = false;

    const bool anyPendingState =
        g_IsEquipping.load(std::memory_order_relaxed) ||
        g_EquipTimer.load(std::memory_order_relaxed) > 0.0f ||
        g_StopTimer.load(std::memory_order_relaxed) > 0.0f;

    // Detect movement stop transition: arm the stop buffer timer once.
    // Turn animations only fire when standing still — skip while moving freely.
    if (s_WasMoving && !isMoving) {
      g_StopTimer.store(0.166f);  // ~166ms regardless of FPS
      SKSE::log::info(">>> Movement stopped. Starting 0.166s stop timer.");
    }
    s_WasMoving = isMoving;

    if (!isMoving || anyPendingState) {

      // 3. Inject "turnStop"
      bool injectTurnStop = false;

      // Equip / unequip handling
      if (g_IsEquipping.load()) {
        injectTurnStop = true;
      } else if (g_EquipTimer.load() > 0.0f) {
        injectTurnStop = true;
        g_EquipTimer.fetch_sub(a_delta, std::memory_order_relaxed);
      }

      // Post-movement stop buffer
      if (g_StopTimer.load() > 0.0f) {
        injectTurnStop = true;
        g_StopTimer.fetch_sub(a_delta, std::memory_order_relaxed);
      }

      // Spam turnStop event.
      // Cache the BSFixedString once — avoid per-frame hashing (Global Rule 3).
      if (injectTurnStop) {
        static const RE::BSFixedString turnStopEvent("turnStop");
        a_this->NotifyAnimationGraph(turnStopEvent);
      }
    }
  }

  // Original Update
  _Update(a_this, a_delta);
}

// ---------------------------------------------------------------------------
// Helper: stristr
// ---------------------------------------------------------------------------
const char *PlayerHook::stristr(const char *str1, const char *str2) {
  if (!*str2)
    return str1;
  for (; *str1; ++str1) {
    if (toupper(static_cast<unsigned char>(*str1)) ==
        toupper(static_cast<unsigned char>(*str2))) {
      const char *h, *n;
      for (h = str1, n = str2; *h && *n; ++h, ++n) {
        if (toupper(static_cast<unsigned char>(*h)) !=
            toupper(static_cast<unsigned char>(*n)))
          break;
      }
      if (!*n)
        return str1;
    }
  }
  return nullptr;
}

} // namespace Hooks
