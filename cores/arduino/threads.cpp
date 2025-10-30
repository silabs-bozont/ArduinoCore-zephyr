/*
 * Copyright (c) 2025 Arduino SA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Arduino.h"

#ifdef CONFIG_MULTITHREADING
void start_static_threads() {
#define _FOREACH_STATIC_THREAD(thread_data)                                                        \
	STRUCT_SECTION_FOREACH (_static_thread_data, thread_data)

	k_timeout_t timeout;
	timeout.ticks = 0u;

	_FOREACH_STATIC_THREAD(thread_data) {
		k_thread_create(thread_data->init_thread, thread_data->init_stack,
						thread_data->init_stack_size, thread_data->init_entry, thread_data->init_p1,
						thread_data->init_p2, thread_data->init_p3, thread_data->init_prio,
						thread_data->init_options, timeout);
		k_thread_name_set(thread_data->init_thread, thread_data->init_name);
		thread_data->init_thread->init_data = thread_data;
	}
}
#endif
