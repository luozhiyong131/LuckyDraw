#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSet>
#include <QVector>
#include <QString>
#include <QTimer>
#include <QLabel>


// 奖项配置结构体
struct PrizeConfig
{
    int id;               // 抽奖顺序 (数字 1, 2, 3...)
    QString displayName;  // 奖项名称 (如：一等奖)
    QString priceName;    // 奖品名称 (如：苹果手机)
    QString image;        // 图片路径
    int winnersPerRound = 0; // 每轮抽取人数
    int totalRounds = 0;     // 总轮数
    bool allowUsedPeople;
};

struct Person
{
    QString employeeId;
    QString name;
};

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

protected:
    void resizeEvent(QResizeEvent *event) override;

    void saveWinnersToTxt(const QString &prizeName, const QVector<Person> &winners);
    void loadHistoryFromTxt();

    // 数据加载逻辑
    void loadPeopleCsv(const QString &path);
    void loadPrizeConfig(const QString &path);
    void updateRoundUI();
    // UI 更新逻辑
    void updatePrizeUI();
    void processRoundLogic();
    QSet<QString> getIdsFromCurrentSection(const QString &prizeName);
    void prepareDynamicLabels(int availableCount);
    void initUIState();
    void savePrizeConfigToJson();


private slots:
    void on_startDrawButton_clicked();
    void onTimerTimeout();
    void drawFinalWinners();
    //void handleAnimation(); // 定时器槽函数，处理名字闪烁

    void on_btnSettings_clicked();

private:
    Ui::Widget *ui;
    void setFullBackground();

    QVector<Person> getCurrentPool();

    // 抽奖相关数据
    QSet<QString> m_usedPeople;   // 记录已中奖的工号，防止重复中奖
    QVector<Person> m_people;    // 全体名单
    QVector<PrizeConfig> m_prizes; // 奖项列表
    int m_currentPrizeIndex = 0; // 当前正在抽第几个奖

    // 动画控制
    QTimer *m_timer = nullptr;   // 用于名字滚动
    bool m_isRunning = false;    // 标识是否正在滚动
    int m_currentRound = 1;


    bool m_isPendingPrizeSwitch = false; // 是否处于等待切换奖项的状态
    QVector<QLabel*> m_dynamicLabels; // 存放动态创建的名牌
};

#endif // WIDGET_H
