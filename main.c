#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "embedded_cli.h"

static bool exitFlag = false;

static void onCommand(const char* name, char *tokens)
{
    printf("Received command: %s\r\n",name);

    for (int i = 0; i < embeddedCliGetTokenCount(tokens); ++i) {
        printf("Arg %d : %s\r\n", i, embeddedCliGetToken(tokens, i + 1));
    }
}

static void onExit(EmbeddedCli *cli, char *args, void *context)
{
    (void)cli;
    (void)args;
    (void)context;
    exitFlag = true;
    printf("Cli will shutdown now...\r\n");
}

static void onHello(EmbeddedCli *cli, char *args, void *context)
{
    (void)cli;
    printf("Hello, ");
    if (embeddedCliGetTokenCount(args) == 0)
        printf("%s", (const char *) context);
    else
        printf("%s", embeddedCliGetToken(args, 1));
    printf("\r\n");
}

static void onLed(EmbeddedCli *cli, char *args, void *context)
{
    (void)cli;
    (void)args;
    (void)context;
    printf("Current led brightness: %d\r\n", rand() % 256);
}

static void onAdc(EmbeddedCli *cli, char *args, void *context)
{
    (void)cli;
    (void)args;
    (void)context;
    printf("Current adc readings: %d\r\n", rand() % 104);
}

static void onCommandFn(EmbeddedCli *embeddedCli, CliCommand *command)
{
    (void)embeddedCli;
    embeddedCliTokenizeArgs(command->args);
    onCommand(command->name == NULL ? "" : command->name, command->args);
}

static void writeCharFn(EmbeddedCli *embeddedCli, char c)
{
    (void)embeddedCli;
    putchar(c);
}

int main()
{
    // Initialize the console
    stdio_init_all();
    printf("pico-embedded-cli\r\n");
    while(getchar_timeout_us(0) != PICO_ERROR_TIMEOUT) {
        // flush out the console input buffer
    }

    // Initialize the CLI
    EmbeddedCli *cli = embeddedCliNewDefault();
    cli->onCommand = onCommandFn;
    cli->writeChar = writeCharFn;
    CliCommandBinding exitBinding = {
            "exit",
            "Stop CLI and exit",
            false,
            NULL,
            onExit
    };
    embeddedCliAddBinding(cli, exitBinding);
    CliCommandBinding ledBinding = {
            "get-led",
            "Get current led status",
            false,
            NULL,
            onLed
    };
    embeddedCliAddBinding(cli, ledBinding);
    CliCommandBinding adcBinding = {
            "get-adc",
            "Get current adc value",
            false,
            NULL,
            onAdc
    };
    embeddedCliAddBinding(cli, adcBinding);
    CliCommandBinding helloBinding = {
            "hello",
            "Print hello message",
            true,
            (void *) "World",
            onHello
    };
    embeddedCliAddBinding(cli, helloBinding);

    printf("Cli is running. Press 'Esc' to exit\r\n");
    printf("Type \"help\" for a list of commands\r\n");
    printf("Use backspace and tab to remove chars and autocomplete\r\n");

    embeddedCliProcess(cli);
     while (!exitFlag) {
        int c = getchar_timeout_us(0);
        if (c != PICO_ERROR_TIMEOUT) {
            //printf("c=0x%X\r\n", c);
            if (c == '\e') {
                // escape key pressed
                exitFlag = true;
                printf("Cli will shutdown now...\r\n");
            }
            else {
                embeddedCliReceiveChar(cli, c);
                embeddedCliProcess(cli);
            }
        }
    }
    return 0;
}
