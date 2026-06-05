#include "Hooks.h"
#include "PCH.h"
#include <spdlog/sinks/basic_file_sink.h>

namespace {
void InitializeLog() {
  auto path = SKSE::log::log_directory();
  if (!path) {
    return;
  }

  *path /= "DisableTurnAnimation.log";
  try {
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        path->string(), true);
    auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

    log->set_level(spdlog::level::info);
    log->flush_on(spdlog::level::info);

    spdlog::set_default_logger(std::move(log));
    spdlog::set_pattern("[%H:%M:%S.%e] [%n] [%l] %v"s);

    SKSE::log::info("Disable Turn Animation log initialized");
  } catch (const std::exception &) {
    // Fail silently but allow plugin to load
  }
}

void OnMessaging(SKSE::MessagingInterface::Message *a_msg) {
  if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
    SKSE::log::info("Disable Turn Animation: Data loaded. Installing hooks...");
    Hooks::PlayerHook::Install();
  }
}
} // namespace

SKSEPluginInfo(.Version = {1, 0, 0, 0}, .Name = "Disable Turn Animation",
               .Author = "Smooth",
               .RuntimeCompatibility =
                   SKSE::VersionIndependence::AddressLibrary)

    SKSEPluginLoad(const SKSE::LoadInterface *a_skse) {
  InitializeLog();
  SKSE::log::info("Disable Turn Animation loaded");

  SKSE::Init(a_skse);

  auto messaging = SKSE::GetMessagingInterface();
  if (messaging) {
    messaging->RegisterListener(OnMessaging);
  }

  return true;
}
