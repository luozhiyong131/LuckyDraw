#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSet>
#include <QVector>
#include <QString>
#include <QTimer>
#include <QLabel>
#include <QElapsedTimer>

/**
 * @brief 奖项配置结构体
 * 用于存储从 prizes.json 中读取的奖项信息
 */
struct PrizeConfig
{
    int id;                  // 抽奖顺序编号 (数字 1, 2, 3...)
    QString displayName;     // 奖项显示名称 (例如：一等奖)
    QString priceName;       // 奖品具体名称 (例如：华为手机)
    QString image;           // 奖品图片在本地的相对路径
    int winnersPerRound = 0; // 每一轮抽取的名额数量
    int totalRounds = 0;     // 该奖项总共抽取的轮数
    bool allowUsedPeople;    // 是否允许已中奖的人重复参与（全员奖项标识）
};

/**
 * @brief 人员信息结构体
 * 对应 Excel/CSV 中的每一行数据
 */
struct Person
{
    QString employeeId;      // 员工工号 (唯一标识)
    QString name;            // 员工姓名
};

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    /** @brief 构造函数：初始化系统状态、加载数据、设置 UI */
    explicit Widget(QWidget *parent = nullptr);

    /** @brief 析构函数：清理资源 */
    ~Widget() override;

protected:
    /** @brief 窗口尺寸改变事件：用于动态调整背景图大小以适配全屏 */
    void resizeEvent(QResizeEvent *event) override;

    /** @brief 关闭窗口提示 */
    void closeEvent(QCloseEvent *event) override;

    /** @brief 将当次中奖名单保存至 winners_config.txt，支持断电恢复 */
    void saveWinnersToTxt(const QString &prizeName, const QVector<Person> &winners);

    /** @brief 程序启动时加载历史记录：填补已中奖黑名单并恢复抽奖进度 */
    void loadHistoryFromTxt();

    /** @brief 读取 CSV 格式的人员名单 (备用函数) */
    void loadPeopleCsv(const QString &path);

    /** @brief 读取 Excel (.xlsx) 格式的人员名单 (主用函数) */
    void loadPeopleExcel(const QString &path);

    /** @brief 从 JSON 文件加载奖项配置信息 */
    void loadPrizeConfig(const QString &path);

    /** @brief 刷新界面上的轮数显示 (如：第 1 / 3 轮) */
    void updateRoundUI();

    /** @brief 刷新界面上的奖项名称、奖品名称和奖品图片 */
    void updatePrizeUI();

    /** @brief 核心业务逻辑：判断当前奖项是否完成，决定步进到下一轮还是下一奖项 */
    void processRoundLogic();

    /** @brief 获取当前奖项在历史记录中已中奖的 ID 集合 (用于全员抽奖模式过滤) */
    QSet<QString> getIdsFromCurrentSection(const PrizeConfig &config);

    /** @brief 动态创建右侧中奖/候选格子的布局，根据本轮人数生成 Label */
    void prepareDynamicLabels(int availableCount);

    /** @brief 初始化界面初始状态 (隐藏统计标签、设置按钮文字等) */
    void initUIState();

    /** @brief 将内存中修改后的奖项列表强制写入 prizes.json 磁盘文件 */
    void savePrizeConfigToJson();

private slots:
    /** @brief 抽奖主按钮点击事件：处理开始滚动、停止计算、切换奖项三种状态 */
    void on_startDrawButton_clicked();

    /** @brief 滚动定时器槽函数：利用洗牌算法实现屏幕上的不重复随机名字滚动 */
    void onTimerTimeout();

    /** @brief 最终结果计算：按下停止后产生真正的随机中奖人，并应用大字体渲染 */
    void drawFinalWinners();

    /** @brief 设置按钮槽函数：处理隐藏的 5 次连点进入配置逻辑 */
    void on_btnSettings_clicked();

private:
    Ui::Widget *ui;

    /** @brief 设置自适应比例的背景图片 */
    void setFullBackground();

    /** @brief 计算当前环境下所有符合抽奖资格的人员名单 */
    QVector<Person> getCurrentPool();

    // --- 核心数据存储 ---
    QSet<QString> m_usedPeople;    // 全局黑名单：记录所有非全员模式下已中奖的工号
    QVector<Person> m_people;      // 名单总表：存储 Excel 中加载的所有人
    QVector<PrizeConfig> m_prizes; // 奖项总表：存储所有配置的奖项
    int m_currentPrizeIndex = 0;   // 状态标识：当前正在抽第几个奖项 (索引)
    int m_currentRound = 1;        // 状态标识：当前奖项的第几轮

    // --- 交互控制 ---
    int m_settingsClickCount = 0;  // 设置按钮点击计数器
    QElapsedTimer m_clickTimer;    // 记录连点设置按钮的时间间隔
    QElapsedTimer m_lastClickTimer;// 主抽奖按钮防抖计时器 (2秒逻辑)

    // --- 动画与状态 ---
    QTimer *m_timer = nullptr;     // 名字滚动的核心定时器
    bool m_isRunning = false;      // 标识当前是否处于滚动动画状态
    bool m_isPendingPrizeSwitch = false; // 标识是否处于“点击进入下一奖项”的等待期
    QVector<QLabel*> m_dynamicLabels;    // 右侧滚动区域的 Label 控件集合

};

#endif // WIDGET_H
