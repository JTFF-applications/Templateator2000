#pragma once

#define WRAPPED_HTML_LABEL(label) (label)->setTextFormat(Qt::RichText);\
								  (label)->setTextInteractionFlags(Qt::TextBrowserInteraction);\
								  (label)->setOpenExternalLinks(true);\
								  (label)->setWordWrap(true);

#define LABEL_ERROR_MSG(label) (label)->setStyleSheet("QLabel { color : red; }");
