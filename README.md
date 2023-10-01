# 【虚幻引擎一起学！】【Dev Enabled - 3D平台类游戏角色动作】Dev Enabled - 3D Platformer Character Movement

![""](Images/Cover.png)

## 【项目学习中】

本项目学习的是Dev Enabled大佬发布在YouTube上的教程。

该教程的主要内容是3D平台类游戏的角色动作的功能实现，主要使用的是蓝图来编写逻辑。

阿严将在学习过程中尝试将大部分的逻辑都转成C++，并实现C++和蓝图之间较好的通信。

因此，本学习项目的一个重点就是如何结合C++以及蓝图，尽力做到这两者之间的均衡。

本项目的另一个学习重点则是动画蓝图（Animation Blueprint）相关的知识。

---
---

- 本项目GitHub仓库：<https://github.com/AtCloudStudio/LetsLearnUnrealEngineTogether_DevEnabled_3DPlatformerCharacterMovement>

---

- Dev Enabled的YouTube频道：<https://www.youtube.com/@DevEnabled>
- 本项目所学习的教程播放列表：<https://youtube.com/playlist?list=PL9z3tc0RL6Z5Yi7-W8qxjrzTb6tHS_UAK&si=HZbqnjORa4u3Da9l>
- 本项目初始工程文件下载: <https://devenabled.itch.io/devvy-channel-mascot-project>

---

- 阿严的YouTube频道：<https://www.youtube.com/@RYanIndieDev/channels>
- 阿严的B站主页：<https://space.bilibili.com/27164588/>

---

【帮助我走得更远，阿严需要你的赞助】

- Patreon: <https://www.patreon.com/RYanIndieDev>
- 爱发电：<https://afdian.net/a/RYanIndieDev>

---
---

【Ep.01】

- 直播时间：2023年9月27日
- YouTube: <https://youtube.com/live/crXqddb5vjg>
- Bilibili: <https://www.bilibili.com/video/BV16m4y1V7mY>

主要内容：

1. 创建新项目，项目设置和编辑器偏好设置
2. 虚幻引擎里如何进行资产的迁移与导出
3. 创建增强输入(Enhanced Input)的输入动作文件(Input Actions)与输入映射上下文(Input Mapping Context)文件
4. 输入按键绑定
5. 初识角色动作组件(Character Movement Component)
6. 使用角色动作组件实现角色移动与镜头控制（蓝图逻辑）

---

【Ep.02】

- 直播时间：2023年9月28日
- YouTube: <https://youtube.com/live/LuAcJ3nx3T8>
- Bilibili: <https://www.bilibili.com/video/BV1rN4y1Z73R>

主要内容：

1. 如何将蓝图逻辑转换成C++源码
2. 如何实现C++与蓝图之间的通信（互相调用）
3. 实现基础的跳跃(Basic Jump)
4. 实现小跳(Variable Height Jump)
5. 实现二段跳(Double Jump)
6. 实现慢走(Walk)

---

【Ep.03】

- 直播时间：2023年9月29日
- YouTube: <https://youtube.com/live/YxMMjOicMU0>
- Bilibili: <https://www.bilibili.com/video/BV1Uw411m7AN>

主要内容：

1. 实现冲刺(Sprint)
2. 如何实现蹲伏(Crouch)
3. 创建动画蓝图(Anim Blueprint)
4. 创建自定义动画蓝图C++类
5. 使用动画状态机(State Machine)
6. 使用混合空间(Blend Space)实现平滑自然的动画转换

---

【Ep.04】

- 直播时间：2023年9月30日
- YouTube: <https://youtube.com/live/Twnhjui2nkg>
- Bilibili: <>

主要内容：

1. 地面各个动画状态（空闲Idle/走路Walk/跑步Run/冲刺Sprint）之间的切换
2. 添加跳跃(JumpUp)动画状态
3. 添加掉落(Fall)动画状态
4. 添加着地(Land)动画状态（使用混合空间）
5. 添加二段跳(Air Jump)动画状态
6. Bug修复
7. 手感和动画的微调
8. 动画蒙太奇(Anim Montage)的概念和原理

---

【Ep.05】

- 直播时间：2023年10月01日
- YouTube: <https://youtube.com/live/sjpTsHjzCXs>
- Bilibili: <>

主要内容：

1. 墙壁检测(WallCheck)的实现
2. 两种不同的检测方法：CapsuleTraceByChannel节点与SweepSingleByChannel
3. 对象标签(Actor Tag)与标签的作用
4. 滑墙(Wall Slide)的实现
5. 添加滑墙动画状态
6. 向量插值运算函数VInterpTo()
7. 如何在C++里实现类似蓝图节点DoOnce功能
