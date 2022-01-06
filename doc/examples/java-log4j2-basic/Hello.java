/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (C) 2022 EfficiOS Inc.
 */

import java.io.IOException;

import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;

/**
 * Example application using the LTTng-UST Java log4j agent.
 *
 * <p>
 * To obtain LTTng trace events, you should run the following sequence of
 * commands:
 * </p>
 *
 * <ul>
 * <li>$ lttng create</li>
 * <li>$ lttng enable-event -l -a</li>
 * <li>$ lttng start</li>
 * <li>(run this program)</li>
 * <li>$ lttng stop</li>
 * <li>$ lttng view</li>
 * <li>$ lttng destroy</li>
 * </ul>
 *
 * @author Alexandre Montplaisir
 * @author Christian Babeux
 */
public class Hello {

	private static final Logger logger = LogManager.getLogger(Hello.class);

	/**
	 * Application start
	 *
	 * @param args
	 *            Command-line arguments
	 * @throws IOException
	 *             If the required native libraries cannot be found. You may
	 *             have to specify "-Djava.library.path=..." on the "java"
	 *             command line.
	 */
	public static void main(String args[]) throws IOException {

		/* Trigger some tracing events using the Log4j Logger created before. */
		logger.info("Basic config: Hello World, the answer is " + 42);
		logger.info("Basic config: Another info event");
		logger.error("Basic config: An error event");
	}
}

