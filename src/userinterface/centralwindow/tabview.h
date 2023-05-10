/*!
 *  \file   tabview.h
 *  \author Eva Navratilova
 */

#ifndef MAINTABWIDGET_H
#define MAINTABWIDGET_H

#include <QTabWidget>
#include <QGridLayout>
#include <QSettings>

#include "../../kernel.h"
#include "../../textformatter.h"
#include "tabview/tabindividual.h"
#include "tabview/tabnote.h"
#include "tabview/treeview.h"
#include "tabview/tabsource.h"
#include "tabview/tabstatistics.h"

#include "tabview/tabsync.h"

enum class TabViewTabs{
    TAB_INDI = 0,
    TAB_NOTE,
    TAB_TREE,
    TAB_SRC,
    TAB_SYNC,
    TAB_STAT

};

class TabView : public QTabWidget
{
    Q_OBJECT
private:
    Kernel &_kernel;
    Identifier &_proband;
    Identifier &_source;

    QGridLayout _layout;
    TabIndividual* _tabIndividual;
    TabNote* _tabNote;
    TreeView* _tabTree;
    TabSource* _tabSource;
    TabStatistics* _tabStatistics;
    TabSync* _tabSync;

public:
    TabView(Kernel &k, Identifier &p, Identifier &s, QWidget *parent = nullptr);
    ~TabView() override;

    static void defaultSettings();
    void saveSettings() const;
    void clearTree();

    TabViewTabs currentTab();
    void setCurrentTab(TabViewTabs index);

private:
    void fill();
    void setCurrentIndex(int index);

signals:
    void probandChanged(Identifier indi);
    void sourceChanged(Identifier ref);

public slots:
    void update();
    void updateSync();
    void drawTree(TreeType t);
    void drawTreeScene(TreeScene *scene);
    void exportImage(const QString& filename);

private slots:
    void tabChanged(int index);
    void setTabIndiText();

};

#endif // MAINTABWIDGET_H
