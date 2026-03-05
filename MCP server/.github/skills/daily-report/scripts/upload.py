#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
简易上传脚本（测试版）
用途：将生成的 HTML 报告复制到与 SKILL.md 同级的 result/ 目录。
注意：仅用于技能联调测试，非生产用途；不进行网络传输。

用法示例：
    python scripts/upload.py --config assets/config.json --file output.html

参数说明：
    --config  指向 JSON 配置文件，需包含 server.url 与 server.token（测试验证用）。
    --file    要“上传”的本地文件路径（通常是生成的 output.html）。
"""

import argparse
import json
import os
import shutil
import sys


def main():
    parser = argparse.ArgumentParser(description='测试上传脚本：拷贝文件到 result/ 目录')
    parser.add_argument('--config', required=True, help='配置文件路径（JSON）')
    parser.add_argument('--file', required=True, help='待上传的本地文件路径（HTML）')
    args = parser.parse_args()

    # 1) 检查配置文件
    if not os.path.exists(args.config):
        print('[ERROR] 缺少配置文件：%s' % args.config)
        print('请创建 assets/config.json，例如：\n'
              '{\n  "server": {"url": "http://localhost:8000", "token": "TEST_TOKEN"}\n}')
        sys.exit(2)

    try:
        with open(args.config, 'r', encoding='utf-8') as cf:
            cfg = json.load(cf)
        # 仅做字段存在性校验（不打印 token，避免泄漏）
        server = cfg.get('server', {})
        if not server.get('url') or not server.get('token'):
            print('[ERROR] 配置缺少 server.url 或 server.token')
            sys.exit(2)
    except Exception as e:
        print('[ERROR] 读取配置失败：', e)
        sys.exit(2)

    # 2) 检查源文件
    src = args.file
    if not os.path.exists(src):
        print('[ERROR] 待上传文件不存在：%s' % src)
        sys.exit(2)

    # 3) 计算目标目录（与本脚本所在仓库同级的 result/）
    # 约定：scripts/ 与 result/ 在相同层级
    scripts_dir = os.path.dirname(os.path.abspath(__file__))
    repo_root = os.path.dirname(scripts_dir)
    result_dir = os.path.join(repo_root, 'result')
    os.makedirs(result_dir, exist_ok=True)

    # 4) 拷贝到 result/ 下，文件名不变
    dst = os.path.join(result_dir, os.path.basename(src))
    shutil.copy2(src, dst)
    print('[OK] 已拷贝到：', dst)
    print('[NOTE] 测试脚本未进行真实网络传输。')


if __name__ == '__main__':
    main()