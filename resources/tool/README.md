## MultiNet5 EN
####  1. Install g2p_en, please refer to https://pypi.org/project/g2p-en/ 

```
pip install g2p_en
```

#### 2. Run multinet_g2p.py

```
python multinet_g2p.py -t "hello world,hi ESP;turn on the light;turn off the light"

------
in: hello world,hi ESP;turn on the light;turn off the light
out: hcLb WkLD,hi fST;TkN nN jc LiT;TkN eF jc LiT;
```

## MultiNet5 CN
####  1. Install pypinyin and pypinyin_dict

```
pip install pypinyin
pip install pypinyin_dict
```

#### 2. Run multinet_pinyin.py

```
python multinet_pinyin.py -t "你好世界；打开灯光"

------
in: 你好世界；打开灯光
out: ni hao shi jie ； da kai deng guang
```
