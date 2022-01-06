/*
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (C) 2015-2022 EfficiOS Inc.
 * Copyright (C) 2015 Alexandre Montplaisir <alexmonthy@efficios.com>
 */

package org.lttng.ust.agent.log4j2;

import java.util.Collection;
import java.util.Collections;
import java.util.Enumeration;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;
import java.util.Map;

import org.apache.logging.log4j.core.Appender;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.core.Logger;
import org.apache.logging.log4j.core.LoggerContext;

import org.lttng.ust.agent.AbstractLttngAgent;

/**
 * Agent implementation for using the Log4j logger, connecting to a root session
 * daemon.
 *
 * @author Alexandre Montplaisir
 */
class LttngLog4jAgent extends AbstractLttngAgent<LttngLogAppender> {

	private static LttngLog4jAgent instance = null;

	private LttngLog4jAgent() {
		super(Domain.LOG4J);
	}

	public static synchronized LttngLog4jAgent getInstance() {
		if (instance == null) {
			instance = new LttngLog4jAgent();
		}
		return instance;
	}

	@Override
	public Collection<String> listAvailableEvents() {
		Set<String> ret = new TreeSet<String>();

		// FIXME: Might be more than one context? This whole thing is pretty dirty.
		LoggerContext logcontext = (LoggerContext) LogManager.getContext(false);
		Collection<? extends Logger> loggers = logcontext.getLoggers();

		for (Logger logger : loggers) {
			if (logger == null) {
				continue;
			}

			/*
			 * Check if that logger has at least one LTTng log4j appender
			 * attached.
			 */
			if (hasLttngAppenderAttached(logger)) {
				ret.add(logger.getName());
			}
		}

		return ret;
	}

	private static boolean hasLttngAppenderAttached(Logger logger) {
		Map<String, Appender> appenders = logger.getAppenders();
		if (appenders != null) {
			for (Map.Entry<String, Appender> appender : appenders.entrySet()) {
				if (appender.getValue() instanceof LttngLogAppender) {
					return true;
				}
			}
		}

		/*
		 * A parent logger, if any, may be connected to an LTTng handler. In
		 * this case, we will want to include this child logger in the output,
		 * since it will be accessible by LTTng.
		 */
		Logger parent = logger.getParent();
		if (parent != null) {
			return hasLttngAppenderAttached(parent);
		}

		/*
		 * We have reached the root logger and have not found any LTTng handler,
		 * this event will not be accessible.
		 */
		return false;
	}

}
