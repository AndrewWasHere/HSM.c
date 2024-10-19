#pragma once

#ifdef HSM_LOGGING
    // Replace with local system logging when appropriate.
    // For example, in ESP-IDF, this might become
    // #define HSM_LOG(...) ESP_LOGI("HSM", __VA_ARGS__);
    #define HSM_LOG(...) { printf(__VA_ARGS__); printf("\n"); }
#else
    #define HSM_LOG(...)
#endif
