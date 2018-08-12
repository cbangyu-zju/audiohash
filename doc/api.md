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
	32000, // size_t, nsample，采样点个数，采样率*时间长度*通道数，建议输入时间片段：2-5s
)
if res == 0:
	print("模版设置成功")
	
score = audio_compare(  // 输入测试流，得到得分，建议输入时间片段：2-5s
	config,
	1, // size_t, channel, 声道个数，比如左右两声道，或者只有一个声道，建议只有1个声道，最多2个
	16000, // size_t, sample_rate，采样率
	dist_buffer, // float *, buffer，模版的原始流数据，流数据长度应该和采样点数相同
	32000, // size_t, nsample，采样点个数，采样率*时间长度*通道数，建议输入时间片段：2-5s
)

print(score)
```

### 编译流程
```
cd src            // 进入代码目录
cmake ./          // 生成makefile文件
make              // 生成接口文件 audiohash.so
```

### 测试接口文件见 aucompare.cpp 
正式使用该库时，需要把aucompare.cpp 文件移除。

```
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "capi.h"
#include <algorithm>

using namespace std;

void printHelp()
{
    fprintf(stderr, "compare audio \n");
}

int main(int argc, char **argv)
{
    void *config = NULL;
    int error = 0;
    float score = 0;
    size_t nchannel = 1;
    size_t sample_rate = 16000;
    size_t nsample = 24000;
    float buffer[24000] = {16};
    fill_n(buffer, 24000, 16);
    config = new_audiohash_config();

    error = set_pattern_audio(config, nchannel, sample_rate, buffer, nsample);
    if (0 != error)
    {
        delete_audiohash_config(config);
        return -1;
    }

    score = audio_compare(config, nchannel, sample_rate, buffer, nsample);

    /*
    if (0 == score)
    {
        fprintf(stderr, "Error occurs while compare pattern");
    }
    else
    {
        printf("socre: %8.4lf\n", score);
    }
    */
    printf("score: %f\n", score);
    return 0;
}
```

### 外部依赖库
```
libsamplerate
作用：用于降采样，将44.1k采样的数据将采样到8k，然后进行数据分析。
该库在各平台上的编译可以参考：https://github.com/cooado/samplerate
```