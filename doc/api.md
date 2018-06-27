## audiohash

### 接口文件
```
capi.h // 接口API
```

### 调用流程
```
config = new_audiohash_config()
res = set_pattern_audio(  // 设置音频模版流
	config,
	1, // size_t, channel, 声道个数，比如左右两声道，或者只有一个声道，建议只有1个声道，最多2个
	16000, // size_t, sample_rate，采样率
	dist_buffer, // float *, buffer，模版的原始流数据，流数据长度应该和采样点数相同
	32000, // size_t, nsample，采样点个数，采样率*时间长度，建议输入时间片段：2-5s
)
if res == 0:
	print("模版设置成功")
	
score = audio_compare(  // 输入测试流，得到得分，建议输入时间片段：2-5s
	config,
	1, // size_t, channel, 声道个数，比如左右两声道，或者只有一个声道，建议只有1个声道，最多2个
	16000, // size_t, sample_rate，采样率
	dist_buffer, // float *, buffer，模版的原始流数据，流数据长度应该和采样点数相同
	32000, // size_t, nsample，采样点个数，采样率*时间长度，建议输入时间片段：2-5s
)

print(score)
```

### 编译流程
```
cd src            // 进入代码目录
cmake ./          // 生成makefile文件
make              // 生成接口文件 audiohash.so
```
