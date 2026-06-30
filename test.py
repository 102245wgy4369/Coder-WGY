import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv(r"D:\MyApps\WPS Office\2026年6月收入支出.csv")
# 设置中文显示
plt.rcParams["font.sans-serif"] = "SimHei"
plt.rcParams["axes.unicode_minus"] = False

data["日期"] = pd.to_datetime(data["日期"],format="%Y年%m月%d日")

'''
plt.plot(data["日期"],data["支出"],color="orange",marker="o",label="支出")
plt.xlabel("日期")
plt.ylabel("支出")
plt.legend(loc="upper left")

plt.twinx()

plt.bar(data["日期"],data["收入"],color="skyblue",label="收入")
plt.ylabel("收入")
plt.legend()
'''

data["支出"]=data["支出"]/100
data["收入"]=data["收入"]/100

# 创建画布与左轴（支出折线）
fig,ax1=plt.subplots(figsize=(12,6))
line=ax1.plot(data["日期"],data["支出"],color="orange",marker="o",label="支出")
plt.title("收支图")
ax1.set_xlabel("日期")
ax1.set_ylabel("支出",color="orange",fontsize=12)
ax1.legend(loc="upper left")

# 创建右轴（收入柱状图）
ax2=ax1.twinx()
bar=ax2.bar(data["日期"],data["收入"],color="skyblue",label="收入")
ax2.set_ylabel("收入",color="deepskyblue",fontsize=12)
ax2.legend(loc="upper right")

# 自动旋转x轴日期，防止文字重叠
plt.xticks(rotation=45)
plt.tight_layout()

plt.show()
