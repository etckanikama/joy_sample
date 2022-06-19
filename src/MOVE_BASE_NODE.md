# これは何？
move_base_node.cppを動かすためのドキュメント</br>
あとはちょっとした説明とかメモとか


# move_baseとは
簡単にいうと、目的の止まりたい座標と向きを与えるだけで、その座標に向かうための速度指令をいい感じに出してくれるすごいやつ。このとき、壁とかもいい感じに避けてくれるし、わりとなめらかに動いてくれる。</br>
GUIでやるとrviz上で2d_nav_goalするとそこにいい感じに行ってくれる仕組み

# 必要なもの
- ros-noeticのinstall
- move_baseのinstall方法も書かなきゃなぁ
- yamasemi_simのフォルダ
- urg_nodeのフォルダ

# ノードを実行して動かす方法
```
cd ~/catkin_ws

roslaunch yamasemi_sim amcl_yamasemi_2022.launch 

ros run joy_sample move_base_node
```

# pubによる動かし方

プログラムをまだ作ってなくても、move_baseそのものを一回、動かしてぇ〜ってなったときにやると便利（rostopic pub トピック名　その型　おくりたいデータ)

```
cd ~/catkin_ws

roslaunch yamasemi_sim amcl_yamasemi_2022.launch 
```
Goalまで手打ちしして、タブ補完すれば以下のように出てくるので、数値を入力（デフォルトは全部 0）

```
rostopic pub /move_base/goal move_base_msgs/MoveBaseActionGoal "header:
  seq: 0
  stamp:
    secs: 0
    nsecs: 0
  frame_id: ''
  stamp:
    secs: 0
    nsecs: 0
  id: ''
goal:
  target_pose:
    header:
      seq: 0
      stamp:
        secs: 0
        nsecs: 0
      frame_id: 'beego/map'
    pose:
      position:
        x: 1.0
        y: 1.0
        z: 0.0
      orientation:
        x: 0.0
        y: 0.0
        z: 0.0
        w: 1.0" 

```
今回のシミュレーション環境で動かすためには、goal.target_pose.header.frame_idをbeego/mapに変更</br>
このとき、座標(1,1)に動かしたいときは、goal.target_pose.pose.position.x, goal.target_pose.pose.position.yを書き換える</br>
あとはorientationは向きに関する指令値で、x,yで向きを変えられる。x,y,zが0のときは、もともとロボットが向いていた姿勢になるように動いてくれる。このとき、w=1にしておく（理由はわからない）

# tipis

以下は、自分に対するメモ的なやつ</br>
pubやノードを実行してgoalの座標の指令値を送ったときに現在のgoalとかstatusを確認する方法

単純にrostopic listとかすれば、current_goalとか、statusというのが出ているので、それをechoでみてみれば現在の状況わかるんじゃねってやつ

```
rostopic echo /move_base/status
```

```
rostopic echo /move_base/current_goal
```

基本的に、rosのプログラム書くときに必要なのは、topic名とそのmsg型なので、それを確認する。pubでやってることと一緒。</br>
今回であれば、move_base/goalというtopic名がgoal情報を座標としてmove_baseにpublishする役割っぽいので、move_base/goalのmsg型を調べる。google検索とかで出てくる。

すると、move_base_msgs/MoveBaseActionGoalというmsg型であることがわかる。</br>

あとは、このmsg型にどんなデータを渡したら良いのかを確認しなくてはいけないので、以下でmsg型のデータを見てみる。
```
rosmsg show move_base_msgs/MoveBaseActionGoal
```
そうするとpubのタブ補完した中身のデータの構造が確認できるので、それに合わせて、プログラム側で入れ子構造的に数値を代入すれば良い。あと、ヘッダーファイルのincludeとCMakeListsの編集も忘れずに。

## move_baseの役割
この辺のやってることをrqt_graghの図を出しながら説明したい

## シミュレーション環境
今実行してるlaunchファイルの説明とか、mapの生成方法とか（yamlファイル）概要だけでも説明したい。

## makeしてるのに編集前のプログラムが実行される問題
#### 原因：ふいに起きたubuntuの時間が変わったことによる問題
-makeのとき以下のwarnが出ているとき
```
clock skew detected. Your build may be incomplete.
```

#### 解決法
以下にしたがって
https://blog.csdn.net/JeSuisDavid/article/details/123473234

```
find . -type f -exec touch {} \;
```
で解決。
以下もいるかも？
https://qiita.com/ma91n/items/73d97a7eb2402457c99e