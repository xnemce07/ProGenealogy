/*!
 *  \file   tabview.cpp
 *  \author Eva Navratilova
 */

#include "tabview.h"

TabView::TabView(Kernel &k, Identifier &p, Identifier &s, QWidget *parent)
    : QTabWidget(parent), _kernel(k), _proband(p), _source(s)
{
    this->setLayout(&_layout);
    _layout.setMargin(0);
    _layout.setContentsMargins(0, 0, 0, 0);

    this->fill();

    connect(this, &TabView::currentChanged, this, &TabView::tabChanged);

    // Connect proband changed
    connect(_tabIndividual, &TabIndividual::probandChanged, this, &TabView::probandChanged);
    connect(_tabTree, &TreeView::probandChanged, this, &TabView::probandChanged);

    // Connect source changed
    connect(_tabIndividual, &TabIndividual::sourceChanged, this, &TabView::sourceChanged);
    connect(_tabSource, &TabSource::sourceChanged, this, &TabView::sourceChanged);

    // Connect data changed
    connect(_tabIndividual, &TabIndividual::dataChanged, this, &TabView::setTabIndiText);

    // Connect importing data to update
//    connect(_tabSync, &TabSync::dataChanged, this, &TabView::probandChanged);
    connect(_tabSync, &TabSync::dataChanged, this, &TabView::update);
}

TabView::~TabView()
{
    _tabIndividual->deleteLater();
    _tabNote->deleteLater();
    _tabTree->deleteLater();
    _tabSource->deleteLater();
    _tabSync->deleteLater();
    //_tabStatistics->deleteLater();
}

void TabView::defaultSettings()
{
    TabIndividual::defaultSettings();
    TabSource::defaultSettings();
    TabSync::defaultSettings();
}

void TabView::saveSettings() const
{
    _tabIndividual->saveSettings();
    _tabSource->saveSettings();
    _tabSync->saveSettings();
}

void TabView::clearTree()
{
    _tabTree->remove();
}

TabViewTabs TabView::currentTab()
{
    return static_cast<TabViewTabs>(this->currentIndex());
}

void TabView::setCurrentTab(TabViewTabs index)
{
    this->setCurrentIndex(toUnderlying(index));
}

void TabView::fill()
{
    // Initilize
    _tabIndividual = new TabIndividual(_kernel, _proband, this);
    _tabNote = new TabNote(_kernel, _proband, this);
    _tabTree = new TreeView(_kernel, _proband, this);
    _tabSource = new TabSource(_kernel, _source, this);
    //_tabStatistics = new TabStatistics(_kernel, this);
    _tabSync = new TabSync(_kernel,_proband ,this);

    // Adjust
    this->setUsesScrollButtons(true);

    // Set layout
    this->addTab(_tabIndividual, "");
    this->addTab(_tabNote, tr("Notes"));
    this->addTab(_tabTree, tr("Family Tree"));
    this->addTab(_tabSource, tr("Sources"));
    this->addTab(_tabSync,tr("Synchronization"));
    //this->addTab(_tabStatistics, tr("Statistics"));
}

void TabView::setCurrentIndex(int index)
{
    QTabWidget::setCurrentIndex(index);
}

void TabView::update()
{
    _tabIndividual->update();
    this->setTabIndiText();

    _tabNote->update();

    _tabTree->changeProband(_proband);

    _tabSource->update();

    _tabSync->update();


}

void TabView::updateSync()
{
    _tabSync->updateSync();
}

void TabView::drawTree(TreeType t)
{
    _tabTree->drawTree(t);

    this->setCurrentTab(TabViewTabs::TAB_TREE);
}

void TabView::drawTreeScene(TreeScene *scene)
{
    if(scene != nullptr)
    {
        _tabTree->drawTree(scene);

        this->setCurrentTab(TabViewTabs::TAB_TREE);
    }
}

void TabView::exportImage(const QString &filename)
{
    _tabTree->exportImage(filename);
}

void TabView::tabChanged(int index)
{
    Q_UNUSED(index);

    //Update sync tab in case, some individual's data changed (data change triggers update only in the individual tab)
    if(this->currentWidget() == _tabSync){
        _tabSync->update();
    }

    /*if(this->currentWidget() == _tabStatistics)
    {
        _tabStatistics->show();
    }*/
}

void TabView::setTabIndiText()
{
    if(_kernel.record()->isIndividual(_proband))
    {
        const int usableWidth = qRound(0.5 * this->width());
        QFont font;
        QFontMetrics fontMetrics(font);
        QString text = TextFormatter::getPersonLineText(_kernel, _proband);
        const QString elidedText = fontMetrics.elidedText(text, Qt::ElideRight, usableWidth);
        this->setTabText(toUnderlying(TabViewTabs::TAB_INDI), elidedText);
    }
    else
    {
        this->setTabText(toUnderlying(TabViewTabs::TAB_INDI), tr("No Person"));
    }
}
