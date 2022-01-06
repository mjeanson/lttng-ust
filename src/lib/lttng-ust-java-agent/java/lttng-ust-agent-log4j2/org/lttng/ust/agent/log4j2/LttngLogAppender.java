/*
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (C) 2015-2022 EfficiOS Inc.
 * Copyright (C) 2015 Alexandre Montplaisir <alexmonthy@efficios.com>
 * Copyright (C) 2014 Christian Babeux <christian.babeux@efficios.com>
 */

package org.lttng.ust.agent.log4j2;

import java.io.IOException;
import java.io.Serializable;
import java.util.Collection;
import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.atomic.AtomicLong;

import org.apache.logging.log4j.core.Appender;
import org.apache.logging.log4j.core.Core;
import org.apache.logging.log4j.core.Filter;
import org.apache.logging.log4j.core.LogEvent;
import org.apache.logging.log4j.core.appender.AbstractAppender;
import org.apache.logging.log4j.core.config.plugins.Plugin;
import org.apache.logging.log4j.core.config.plugins.PluginAttribute;
import org.apache.logging.log4j.core.config.plugins.PluginElement;
import org.apache.logging.log4j.core.config.plugins.PluginFactory;

import org.lttng.ust.agent.ILttngAgent;
import org.lttng.ust.agent.ILttngHandler;
import org.lttng.ust.agent.context.ContextInfoSerializer;

/**
 * LTTng-UST Log4j 2.x log handler.
 *
 * Applications can attach this appender to their
 * {@link org.apache.log4j.Logger} to have it generate UST events from logging
 * events received through the logger.
 *
 * It sends its events to UST via the JNI library "liblttng-ust-log4j-jni.so".
 * Make sure this library is available before using this appender.
 *
 * @author Alexandre Montplaisir
 * @author Christian Babeux
 */
@Plugin(name = LttngLogAppender.PLUGIN_NAME, category = Core.CATEGORY_NAME, elementType = Appender.ELEMENT_TYPE, printObject = false)
public final class LttngLogAppender extends AbstractAppender implements ILttngHandler {

	/**
	 * The name of the appender in the configuration.
	 */
	public static final String PLUGIN_NAME = "Lttng";

	private static final String SHARED_OBJECT_NAME = "lttng-ust-log4j2-jni";

	private final AtomicLong eventCount = new AtomicLong(0);

	private final ILttngAgent<LttngLogAppender> agent;


	/**
	 * Constructor
	 *
	 * FIXME: Handle exceptions according to ignoreExceptions param?
	 *
	 * @throws IOException
	 *             This handler requires the lttng-ust-log4j-jni.so native
	 *             library, through which it will send the trace events. This
	 *             exception is throw is this library cannot be found.
	 * @throws SecurityException
	 *             We will forward any SecurityExcepion that may be thrown when
	 *             trying to load the JNI library.
	 */
	protected LttngLogAppender(String name,
			Filter filter,
			boolean ignoreExceptions) throws IOException, SecurityException {

		super(name, filter, null, ignoreExceptions);

		/* Initialize LTTng UST tracer. */
		try {
			System.loadLibrary(SHARED_OBJECT_NAME); // $NON-NLS-1$
		} catch (UnsatisfiedLinkError e) {
			throw new IOException(e);
		}

		/* Register to the relevant agent */
		agent = LttngLog4jAgent.getInstance();
		agent.registerHandler(this);
	}

	@PluginFactory
	public static LttngLogAppender createAppender(
			@PluginAttribute("name") String name,
			@PluginElement("Filters") Filter filter) throws IOException, SecurityException {

		if (name == null) {
			LOGGER.error("No name provided for LttngLogAppender");
			return null;
		}

		return new LttngLogAppender(name, filter, true);
	}

	@Override
	public synchronized void close() {
		agent.unregisterHandler(this);
	}

	/**
	 * Get the number of events logged by this handler so far. This means the
	 * number of events actually sent through JNI to UST.
	 *
	 * @return The number of events logged so far
	 */
	@Override
	public long getEventCount() {
		return eventCount.get();
	}

	@Override
	public void append(LogEvent event) {
		/*
		 * Check if the current message should be logged, according to the UST
		 * session settings.
		 */
		if (!agent.isEventEnabled(event.getLoggerName())) {
			return;
		}

		String classname = "";
		String methodname = "";
		String filename = "";
		int line = -1;

		StackTraceElement ste = event.getSource();
		if (ste != null) {
			classname = ste.getClassName();
			methodname = ste.getMethodName();
			filename = ste.getFileName();
			line = ste.getLineNumber();
		}

		/* Retrieve all the requested context information we can find */
		Collection<Entry<String, Map<String, Integer>>> enabledContexts = agent.getEnabledAppContexts();
		ContextInfoSerializer.SerializedContexts contextInfo = ContextInfoSerializer.queryAndSerializeRequestedContexts(enabledContexts);

		eventCount.incrementAndGet();

		LttngLog4jApi.tracepointWithContext(event.getMessage().getFormattedMessage(),
				event.getLoggerName(),
				classname,
				methodname,
				filename,
				line,
				event.getTimeMillis(),
				event.getLevel().intLevel(),
				event.getThreadName(),
				contextInfo.getEntriesArray(),
				contextInfo.getStringsArray());
	}

}
