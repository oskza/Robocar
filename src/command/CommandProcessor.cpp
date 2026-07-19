#include "CommandProcessor.h"
#include "CommandResponseBuilder.h"
#include "json/CommandJsonParser.h"
#include "json/CommandJsonSerializer.h"

CommandProcessor::CommandProcessor(CommandDispatcher &dispatcher) : _dispatcher(dispatcher) {}

bool CommandProcessor::handle(
    const uint8_t *request,
    size_t requestLength,
    char *responseBuffer,
    size_t responseCapacity
) {
    CommandEnvelope command{};
    const CommandError parseError = CommandJsonParser::parse(
        request,
        requestLength,
        command
    );
    CommandResponse response{};
    response.id = command.id;
    if (parseError != CommandError::NONE)
        CommandResponseBuilder::error(response, parseError);
    else
        _dispatcher.dispatch(command, response);
    return CommandJsonSerializer::serialize(response, responseBuffer, responseCapacity);
}
