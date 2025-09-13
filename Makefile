# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: Copyright (C) 2024, Zhu, Qun-Ying <zhu.qunying*gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

.SUFFIX: .c
CC := gcc
C_FLAGS := -Wall -Wextra -flto -Os -s

x-mouse.exe: x-mouse.c
	$(CC) -o $@ $(C_FLAGS) $^
