#pragma once

#include "PCH.h"
#include <atomic>

namespace Hooks {

// ---------------------------------------------------------------------------
// PlayerHook — Simple Event Blocking & Injection
// ---------------------------------------------------------------------------
class PlayerHook {
public:
  static void Install();

private:
  static inline std::uintptr_t _NotifyAnimationGraph_Orig = 0;

  static bool NotifyAnimationGraph(RE::IAnimationGraphManagerHolder *a_holder,
                                   const RE::BSFixedString &a_eventName);

  static void Update(RE::Character *a_this, float a_delta);
  static inline REL::Relocation<decltype(Update)> _Update;

  static const char *stristr(const char *str1, const char *str2);

  // Internal state for event injection
  static inline std::atomic<bool> g_IsEquipping{false};
  static inline std::atomic<float> g_EquipTimer{0.0f};   // seconds (delta-time based)
  static inline std::atomic<float> g_StopTimer{0.0f};    // seconds after movement stops
};

} // namespace Hooks
