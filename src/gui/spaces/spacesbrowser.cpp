﻿/*
 * Copyright (C) by Hannah von Reth <hannah.vonreth@owncloud.com>
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
#include "spacesbrowser.h"
#include "ui_spacesbrowser.h"

#include "spacesdelegate.h"
#include "spacesmodel.h"

#include "gui/models/expandingheaderview.h"
#include "gui/models/models.h"

#include <QCursor>
#include <QMenu>
#include <QSortFilterProxyModel>

using namespace OCC::Spaces;

SpacesBrowser::SpacesBrowser(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SpacesBrowser)
{
    ui->setupUi(this);
    _model = new SpacesModel(this);

    auto *filterModel = new Models::FilteringProxyModel(this);
    filterModel->setSourceModel(_model);
    filterModel->setFilterRole(Models::DataRoles::FilterRole);
    filterModel->setFilterKeyColumn(static_cast<int>(SpacesModel::Columns::Enabled));

    auto *sortModel = new Models::WeightedQSortFilterProxyModel(this);
    sortModel->setSourceModel(filterModel);
    sortModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    sortModel->setWeightedColumn(static_cast<int>(SpacesModel::Columns::Priority));

    ui->tableView->setModel(sortModel);

    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SpacesBrowser::selectionChanged);

    ui->tableView->setItemDelegate(new SpacesDelegate);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    auto *header = new OCC::ExpandingHeaderView(QStringLiteral("SpacesBrowserHeader2"), ui->tableView);
    ui->tableView->setHorizontalHeader(header);
    header->setResizeToContent(true);
    header->setSortIndicator(static_cast<int>(SpacesModel::Columns::Name), Qt::DescendingOrder);
    header->setExpandingColumn(static_cast<int>(SpacesModel::Columns::Name));
    header->hideSection(static_cast<int>(SpacesModel::Columns::WebDavUrl));
    // part of the name (see the delegate)
    header->hideSection(static_cast<int>(SpacesModel::Columns::Subtitle));
    header->hideSection(static_cast<int>(SpacesModel::Columns::Priority));
    header->hideSection(static_cast<int>(SpacesModel::Columns::Enabled));
    header->hideSection(static_cast<int>(SpacesModel::Columns::SpaceId));
    header->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(header, &QHeaderView::customContextMenuRequested, header, [header, this] {
        auto menu = new QMenu(this);
        menu->setAttribute(Qt::WA_DeleteOnClose);
        header->addResetActionToMenu(menu);
        menu->popup(QCursor::pos());
    });
}

SpacesBrowser::~SpacesBrowser()
{
    delete ui;
}

void SpacesBrowser::setAccount(OCC::AccountPtr acc)
{
    _acc = acc;
    if (acc) {
        _model->setSpacesManager(acc->spacesManager());
    }
}

QModelIndex SpacesBrowser::currentSpace()
{
    const auto spaces = ui->tableView->selectionModel()->selectedRows();
    return spaces.isEmpty() ? QModelIndex {} : spaces.first();
}
