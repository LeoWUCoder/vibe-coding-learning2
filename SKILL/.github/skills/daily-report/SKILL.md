name: daily-report
description: 
  当用户请求“写每日总结”或“上传会议记录”时触发。技能将执行三项任务：
  (1) 读取参考文档 references/集团财务手册.md，对内容进行预算与费用相关的合规检查；
  (2) 使用 assets/report_template.html 将总结内容格式化为 HTML；
  (3) 通过 scripts/upload.py 上传生成的报告文件，并使用 assets/config.json 中的配置完成认证。
  若缺失 config.json，则提示用户配置服务器信息。
argument-hint: "[input_file] [options]"
user-invocable: true
disable-model-invocation: false
