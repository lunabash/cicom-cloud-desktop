/*
 * Copyright (C) Fabian Müller <fmueller@owncloud.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#pragma once

#include "abstractsetupwizardpage.h"

namespace Ui {
class LegacyWebFingerSetupWizardPage;
}

namespace OCC::Wizard {

class LegacyWebFingerSetupWizardPage : public AbstractSetupWizardPage
{
    Q_OBJECT

public:
    LegacyWebFingerSetupWizardPage(const QUrl &serverUrl);
    ~LegacyWebFingerSetupWizardPage() noexcept override;

    QString username() const;

    bool validateInput() override;

private:
    ::Ui::LegacyWebFingerSetupWizardPage *_ui;
};

}
