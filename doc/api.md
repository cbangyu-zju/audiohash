## audiohash

### 接口文件
```
capi.h // 接口API
aucompare.cpp // C++的调用sample
```

### 调用流程
```
config = new_audiohash_config()
res = set_pattern_audio(  // 设置音频模版
	config,
	"PCM", // stream_type, 流类型
	2, // channel, 声道个数，比如左右两声道，或者只有一个声道
	32, // sample_resolution， 分辨率，8位/16位/32位，
	44100, // sample_rate，采样率
	dist_buffer, // buffer，模版的原始流数据，
	10000, // nsample，采样点个数
)
if res == 0:
	print("模版设置成功")
	
score = audio_compare(  // 输入测试流，得到得分
	config,
	"PCM", // stream_type, 流类型
	2, // channel, 声道个数，比如左右两声道，或者只有一个声道
	32, // sample_resolution， 分辨率，8位/16位/32位，
	44100, // sample_rate，采样率
	origin_buffer, // buffer，原始流数据，
	10000, // nsample，采样点个数
)

print(score)
```

### 编译流程
```
cd audiohash/src  // 进入代码目录
cmake ./          // 生成makefile文件
make              // 生成接口文件 audiohash.so
```
