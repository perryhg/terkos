//
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//

/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "WirelessEncryptionType.h"

const string WirelessEncryptionType::NONE_ = "none";
const string WirelessEncryptionType::WEP_ = "wep";
const string WirelessEncryptionType::WPA_ = "wpa";
const string WirelessEncryptionType::UNKNOWN_ = "unknown";

const WirelessEncryptionType WirelessEncryptionType::NONE(NONE_);
const WirelessEncryptionType WirelessEncryptionType::WEP(WEP_);
const WirelessEncryptionType WirelessEncryptionType::WPA(WPA_);
const WirelessEncryptionType WirelessEncryptionType::UNKNOWN(UNKNOWN_);
