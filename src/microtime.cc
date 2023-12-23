#include <errno.h>

#include <napi.h>

#if defined(_MSC_VER)
#include <time.h>
#include <windows.h>

typedef VOID(WINAPI *WinGetSystemTime)(LPFILETIME);
static WinGetSystemTime getSystemTime = NULL;

struct timezone {
  int tz_minuteswest;
  int tz_dsttime;
};

/**
 * @brief Get the current time of day.
 * 
 * This function retrieves the current time of day and stores it in the provided timeval structure.
 * The timeval structure contains two members: tv_sec (seconds) and tv_usec (microseconds).
 * 
 * @param tv A pointer to a timeval structure where the current time will be stored.
 * @param tz A pointer to a timezone structure (not used in this implementation).
 * @return 0 on success.
 */
int gettimeofday(struct timeval *tv, struct timezone *tz) {
  FILETIME ft;
  (*getSystemTime)(&ft);
  unsigned long long t = ft.dwHighDateTime;
  t <<= 32;
  t |= ft.dwLowDateTime;
  t /= 10;
  t -= 11644473600000000ULL;
  tv->tv_sec = (long)(t / 1000000UL);
  tv->tv_usec = (long)(t % 1000000UL);

  return 0;
}
#else
#include <sys/time.h>
#endif

Napi::Error ErrnoException(Napi::Env env, int errorno) {
  Napi::Error e = Napi::Error::New(env, strerror(errorno));
  e.Set("syscall", Napi::String::New(env, "gettimeofday"));
  e.Set("errno", Napi::Number::New(env, errno));
  e.Set("code", Napi::Number::New(env, errno));
  return e;
}

/**
 * Returns the current time as a Napi::Value object.
 *
 * @param info The callback information.
 * @return The current time as a Napi::Value object.
 */
Napi::Value Now(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  timeval t;
  int r = gettimeofday(&t, NULL);

  if (r < 0) {
    ErrnoException(env, errno).ThrowAsJavaScriptException();
    return env.Null();
  }

  return Napi::Number::New(env, ((t.tv_sec * 1000000.0) + t.tv_usec));
}

/**
 * Returns a Napi::Value representing the current time as a double.
 *
 * @param info The Napi::CallbackInfo object containing information about the function call.
 * @return A Napi::Value representing the current time as a double.
 */
Napi::Value NowDouble(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  timeval t;
  int r = gettimeofday(&t, NULL);
  if (r < 0) {
    ErrnoException(env, errno).ThrowAsJavaScriptException();
    return env.Null();
  }

  return Napi::Number::New(env, t.tv_sec + (t.tv_usec * 0.000001));
}

/**
 * Returns the current time as a Napi::Value object.
 *
 * @param info The callback information.
 * @return The current time as a Napi::Value object.
 */
Napi::Value NowStruct(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  timeval t;
  int r = gettimeofday(&t, NULL);

  if (r < 0) {
    ErrnoException(env, errno).ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Array array = Napi::Array::New(env, 2);
  array.Set((uint32_t)0, (double)t.tv_sec);
  array.Set((uint32_t)1, (double)t.tv_usec);

  return array;
}

/**
 * @brief Initializes a Napi::Object with the given environment and exports.
 *
 * This function sets up the Napi::Object with the necessary properties and functions.
 *
 * @param env The Napi::Env environment.
 * @param exports The Napi::Object to be initialized.
 * @return The initialized Napi::Object.
 */
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "now"), Napi::Function::New(env, Now));
  exports.Set(Napi::String::New(env, "nowDouble"),
              Napi::Function::New(env, NowDouble));
  exports.Set(Napi::String::New(env, "nowStruct"),
              Napi::Function::New(env, NowStruct));
#if defined(_MSC_VER)
  getSystemTime = (WinGetSystemTime)GetProcAddress(
      GetModuleHandle(TEXT("kernel32.dll")), "GetSystemTimePreciseAsFileTime");
  if (getSystemTime == NULL) {
    getSystemTime = &GetSystemTimeAsFileTime;
  }
#endif
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);