#pragma comment(lib, "netapi32.lib")
#include <napi.h>
#include <windows.h>
#include <LMaccess.h>
#include <lmapibuf.h>

Napi::Value CheckUserExistence(const Napi::CallbackInfo &info)
{
    NET_API_STATUS status;
    DWORD entriesread;
    DWORD totalentries;
    LPBYTE bufptr;
    USER_INFO_0 *UserInfo;
    DWORD resumehandle = 0;
    bool isUserExist = false;

    Napi::Env env = info.Env();

    // Проверка на количество аргументов
    if (info.Length() < 1)
    {
        Napi::TypeError::New(env, "Неправильное количество аргументов")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    // Проверка на то, что аргумент - строка
    if (!info[0].IsString())
    {
        Napi::TypeError::New(env, "Неправильный аргумент").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Получаем указатель на массив символов в UTF-16 формате
    std::u16string username = info[0].As<Napi::String>().Utf16Value().c_str();

    do
    {
        status = NetUserEnum(NULL,                  // получаем пользователей с локального компьютера
                             0,                     // получаем только имя пользователя
                             FILTER_NORMAL_ACCOUNT, // получаем только обыкновенных пользователей
                             &bufptr,               // указатель на буффер
                             MAX_PREFERRED_LENGTH,  // кол-во получаемых байтов
                             &entriesread,          // указатель на переменную, хранящую кол-во полученных пользователей
                             &totalentries,         // указатель на переменную, хранящую общее кол-во пользователей
                             &resumehandle          // указатель на переменную, хранящую место остановки
        );

        if (status != 0 && status != ERROR_MORE_DATA)
        {
            Napi::Error::New(env, "Не удалось получить данные").ThrowAsJavaScriptException();
            return env.Null();
        }

        // Если успешно получены данные или есть еще данные
        UserInfo = (USER_INFO_0 *)bufptr;

        for (unsigned i = 0; i < entriesread; i++)
        {
            std::wstring checkUserStr(UserInfo->usri0_name);
            std::u16string checkUserStr16(checkUserStr.begin(), checkUserStr.end());
            int result = checkUserStr16.compare(username);
            if (checkUserStr16.compare(username) == 0)
            {
                isUserExist = true;
                break;
            }
            UserInfo++;
        }
        NetApiBufferFree(bufptr);
    } while (status == ERROR_MORE_DATA);

    Napi::Boolean result = Napi::Boolean::New(env, isUserExist);
    return result;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "checkUserExistence"), Napi::Function::New(env, CheckUserExistence));
    return exports;
}

NODE_API_MODULE(addon, Init)