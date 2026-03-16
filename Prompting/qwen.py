import os
from dotenv import load_dotenv
import dashscope
from dashscope import Generation

# 1) 加载 .env 文件（需与本脚本在同一目录）
load_dotenv()

# 2) 从环境变量读取并设置给 dashscope
api_key = os.getenv("DASHSCOPE_API_KEY")
if not api_key:
    raise RuntimeError("未在 .env 中找到 DASHSCOPE_API_KEY，请在 .env 写入：DASHSCOPE_API_KEY=你的APIKey")

dashscope.api_key = api_key

# 3) 你的消息体
messages = [
    {'role': 'system', 'content': 'You are a helpful assistant.'},
    {'role': 'user', 'content': '你是谁？'}
]

# 4) 发起调用
response = Generation.call(
    model="qwen-plus",
    messages=messages,
    result_format='message'
)

# 5) 友好打印
# Generation(message) 返回对象通常含有 .output_text 或 .output.choices
print(getattr(response, "output_text", response))