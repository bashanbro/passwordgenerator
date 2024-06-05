# 强密码生成器 passwordgenerator
本程序可以实现纯数字、纯字母、数字字母混合、数字字母符号混合等多种不同密码组合模式，能保存生成的密码和保存时间，再也不用担心忘记密码。

密码生成规则的改进包括，摒弃了matlab的randperm函数进而是采用了C语言更为安全的rand_s函数，每一位密码均是从来所选字符集合中随机提取；支持检测所生成的密码中是否含有连续的数字或字母的正反序列，即0至9，a到z以及A到Z，若勾选【连续数字或字母】，则存在3个及以上连续的数字或字母的正反序列的密码将不会被输出，如密码中有456、9874、abc与HIJK等；支持检测所生成的密码中是否含有连续的键盘字母的正反序列，如asdfghjkl，若勾选【连续键盘字母】，则存在3个及以上连续的键盘字母的正反序列的密码将不会被输出，如密码中有QWER、asdf等

<div align=center><img src="https://github.com/bashancode/passwordgenerator/blob/main/%E5%BC%BA%E5%AF%86%E7%A0%81%E7%94%9F%E6%88%90%E5%99%A8.png" width="608" height="456"/></div>
