require("abc.js");

function onウィンドウ生成時(arg) {
    let a = 1/0;
    デバッグ出力(arg.ウィンドウハンドル);
    デバッグ出力(arg);
    let rand = new System.Random();
    デバッグ出力("Nextだよ" + rand.Next());
}

function onウィンドウ破棄時() {
    デバッグ出力("破棄来たよ!!");
}