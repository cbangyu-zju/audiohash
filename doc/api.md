## audiohash

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
	
score = set_pattern_audio(  // 设置音频模版
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
