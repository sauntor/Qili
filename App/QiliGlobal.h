/* Copyright (C) 2024  Sauntor <sauntor@live.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef QILIGLOBAL_H
#define QILIGLOBAL_H

#include "Config.h"

#include <QtGlobal>

#define QILI_BEGIN  namespace Qili {
#define QILI_END    };

#define QILI_NS_BEGIN(NS)  namespace Qili::NS {
#define QILI_NS_END(NS)    };

namespace Qili {

const bool Released = QILI_RELEASE_BUILD;

    enum QiliFlag { Restart = 'R' + 'S' + 'T' };

    constexpr const char * const ApplicationName = "Qili";
    constexpr const char * const ApplicationVersion = QILI_VERSION;
    constexpr const char * const ApplicationDisplayName = "DisplayName";
    constexpr const char * const OrganizationName = "Sauntor OSS";
    constexpr const char * const OrganizationDomain = "sauntor.me";
    constexpr const char * const UserAgent = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36";



    namespace Uris {
        constexpr const char * const PlayLive = "https://play-live.bilibili.com";
#define BILI_LIEV_BASE_URL "https://live-open.biliapi.com"
        constexpr const char * const LiveApi = BILI_LIEV_BASE_URL;
        constexpr const char * const LiveStart = BILI_LIEV_BASE_URL "/v2/app/start";
        constexpr const char * const LiveEnd = BILI_LIEV_BASE_URL "/v2/app/end";
#undef BILI_LIEV_BASE_URL
        constexpr const char * const GenerateQrcode = "https://passport.bilibili.com/x/passport-login/web/qrcode/generate";
        constexpr const char * const PollQrcode = "https://passport.bilibili.com/x/passport-login/web/qrcode/poll";
        constexpr const char * const GetDanmuInfo = "https://api.live.bilibili.com/xlive/web-room/v1/index/getDanmuInfo";
    };

    namespace SettingKeys {
        constexpr const char * const ColorMode = "color_mode";
        constexpr const char * const Cookies = "cookies";
        constexpr const char * const KeepUser = "keep_user";
        constexpr const char * const KeepRoom = "keep_room";
        constexpr const char * const ReverseLogs = "reverse_logs";
        constexpr const char * const Room = "room";
        constexpr const char * const Lang = "lang";
        constexpr const char * const Voice = "voice";
        constexpr const char * const Volume = "volume";
        constexpr const char * const Pitch = "pitch";
    };

    namespace Widgets {
        constexpr const char * const PROPERTY = "qili-widget";
        constexpr const char * const TitleBar = "titlebar";
        constexpr const char * const TextField = "text-field";
    };
    namespace Buttons {
        constexpr const char * const PROPERTY = "qili-btn";
        constexpr const char * const Radio = "radio";
        constexpr const char * const Primary = "primary";
    }
};

#endif // QILIGLOBAL_H
