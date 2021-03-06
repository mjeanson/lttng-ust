/*
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (C) 2015 EfficiOS Inc.
 * Copyright (C) 2015 Alexandre Montplaisir <alexmonthy@efficios.com>
 */

package org.lttng.ust.agent.jul;

import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;
import java.util.logging.Handler;
import java.util.logging.LogManager;
import java.util.logging.Logger;

import org.lttng.ust.agent.AbstractLttngAgent;

/**
 * Agent implementation for tracing from JUL loggers.
 *
 * @author Alexandre Montplaisir
 */
class LttngJulAgent extends AbstractLttngAgent<LttngLogHandler> {

	private static LttngJulAgent instance = null;

	private LttngJulAgent() {
		super(Domain.JUL);
	}

	public static synchronized LttngJulAgent getInstance() {
		if (instance == null) {
			instance = new LttngJulAgent();
		}
		return instance;
	}

	@Override
	public Collection<String> listAvailableEvents() {
		Set<String> ret = new TreeSet<String>();

		List<String> loggersNames = Collections.list(LogManager.getLogManager().getLoggerNames());
		for (String name : loggersNames) {
			/*
			 * Skip the root logger. An empty string is not a valid event name
			 * in LTTng.
			 */
			if (name.equals("") || name.equals("global")) {
				continue;
			}

			/*
			 * Check if that logger has at least one LTTng JUL handler attached.
			 */
			Logger logger = Logger.getLogger(name);
			if (hasLttngHandlerAttached(logger)) {
				ret.add(name);
			}
		}

		return ret;
	}

	private static boolean hasLttngHandlerAttached(Logger logger) {
		for (Handler handler : logger.getHandlers()) {
			if (handler instanceof LttngLogHandler) {
				return true;
			}
		}

		/*
		 * A parent logger, if any, may be connected to an LTTng handler. In
		 * this case, we will want to include this child logger in the output,
		 * since it will be accessible by LTTng.
		 */
		Logger parent = logger.getParent();
		if (parent != null) {
			return hasLttngHandlerAttached(parent);
		}

		/*
		 * We have reached the root logger and have not found any LTTng handler,
		 * this event will not be accessible.
		 */
		return false;
	}

}
