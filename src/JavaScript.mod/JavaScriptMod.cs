﻿using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using Microsoft.ClearScript;
using Microsoft.ClearScript.V8;
using ゲーム.Helpers;
using ゲーム.Extensions;
using System.Dynamic;
using Microsoft.ClearScript.JavaScript;

public class IJavaScriptMod
{
    public static void onメインウィンドウ生成後(int hWnd)
    {
        ゲーム.StaticLib.onメインウィンドウ生成後(hWnd);
    }

    public static String onフォント要求時()
    {
        return ゲーム.StaticLib.onフォント要求時();
    }
    public static void onメインウィンドウ破棄前()
    {
        ゲーム.StaticLib.onメインウィンドウ破棄前();
    }

    public static String on音楽要求時(String filepath)
    {
        return ゲーム.StaticLib.on音楽要求時(filepath);
    }

    public static String on効果音要求時(String filepath)
    {
        return ゲーム.StaticLib.on効果音要求時(filepath);
    }
    public static String onファイル要求時(String filepath)
    {
        return ゲーム.StaticLib.onファイル要求時(filepath);
    }

    public static String on顔画像要求時(int KaoID)
    {
        return ゲーム.StaticLib.on顔画像要求時(KaoID);
    }

    public static String on姫顔画像要求時(int KaoID)
    {
        return ゲーム.StaticLib.on姫顔画像要求時(KaoID);
    }

    public static String on家宝画像要求時(int PicID)
    {
        return ゲーム.StaticLib.on家宝画像要求時(PicID);
    }

    public static String on家紋画像要求時(int PicID)
    {
        return ゲーム.StaticLib.on家紋画像要求時(PicID);
    }

}

namespace ゲーム.Helpers
{
    internal static class StringHelpers
    {
        private static readonly Regex LineRegex = new Regex(@" ->\s*(.*)\s*\z");

        public static string CleanupStackTrace(string stackTrace)
        {
            var lines = stackTrace.Split('\n');
            lines[1] = LineRegex.Replace(lines[1], match =>
            {
                var str = match.Groups[1].ToString();
                return " -> " + str;
            });

            for (var i = 1; i < lines.Length; i++)
            {
                var line = lines[i];
                if (line.StartsWith("    at"))
                {
                    lines[i] = line.Substring(2);
                }
            }

            return string.Join("\n", lines);
        }
    }
}


namespace ゲーム.Extensions
{
    internal static class ExceptionExtensions
    {
        public static T GetInnerMost<T>(this Exception exception) where T : class
        {
            var testEx = exception;
            T result = null;
            while (testEx != null)
            {
                var targetEx = testEx as T;
                if (targetEx != null)
                {
                    result = targetEx;
                }

                testEx = testEx.InnerException;
            }

            return result;
        }

        public static string GetScriptStack(this Exception exception)
        {
            var error = exception.GetInnerMost<IScriptEngineException>()?.ErrorDetails;
            return error != null ? StringHelpers.CleanupStackTrace(error) : exception.Message;
        }
    }
}


namespace ゲーム
{
    internal class StaticLib
    {
        private static V8ScriptEngine engine;
        private static 烈風伝 rpdobj;
        private static JSConsole console;

        private static void OutputDebugStream(string message)
        {
            System.Diagnostics.Trace.WriteLine(message);
        }

        public static void debuginfo(params Object[] expressions)
        {
            List<String> list = new List<String>();
            foreach (var exp in expressions)
            {
                bool isClearScriptItem = false;
                try
                {
                    if (exp.GetType().Name == "WindowsScriptItem" || exp.GetType().BaseType?.Name == "WindowsScriptItem" || exp.GetType().BaseType?.BaseType?.Name == "WindowsScriptItem")
                    {
                        isClearScriptItem = true;
                    }
                }
                catch (Exception)
                {

                }
                // WindowsScriptItemエンジンのオブジェクトであれば、そのまま出しても意味が無いので…
                if (isClearScriptItem)
                {
                    dynamic dexp = exp;

                    // JSON.stringifyで変換できるはずだ
                    String strify = "";
                    try
                    {
                        strify = engine.Script.JSON.stringify(dexp);
                        list.Add(strify);
                    }
                    catch (Exception)
                    {

                    }

                    // JSON.stringfyで空っぽだったようだ。
                    if (strify == String.Empty)
                    {
                        try
                        {
                            // ECMAScriptのtoString()で変換出来るはずだ…
                            list.Add(dexp.toString());
                        }
                        catch (Exception)
                        {
                            // 変換できなかったら、とりあえず、しょうがないので.NETのToStringで。多分意味はない。
                            list.Add(exp.ToString());
                        }
                    }
                }

                // WindowsScriptItemオブジェクトでないなら、普通にToString
                else
                {
                    list.Add(exp.ToString());
                }
            }

            String joind = String.Join(" ", list);
            OutputDebugStream(joind);
        }

        public static void onメインウィンドウ生成後(int hWnd)
        {
            try
            {
                DoFile("JavaScript.mod.js");
                dynamic arg = new ExpandoObject();
                dynamic ret = new ExpandoObject();
                arg.ウィンドウハンドル = hWnd;
                engine.Script.onメインウィンドウ生成後(arg, ret);
            }
            catch (Exception e)
            {
                OutputDebugStream("onメインウィンドウ生成後Error:" + e.Message);
            }
        }

        public static void onメインウィンドウ破棄前()
        {
            if (engine != null)
            {
                try
                {
                    dynamic ret = new ExpandoObject();
                    dynamic arg = new ExpandoObject();
                    engine.Script.onメインウィンドウ破棄前(arg, ret);
                }
                catch (Exception e)
                {
                    OutputDebugStream("onメインウィンドウ破棄前Error:" + e.Message);
                }
            }
            DestroyEngine();
        }



        public static String onフォント要求時()
        {
            try
            {
                dynamic ret = new ExpandoObject();
                dynamic arg = new ExpandoObject();
                engine.Script.onフォント要求時(arg, ret);
                if (ret is Undefined)
                {
                    return "";
                }
                if (ret.フォント名 is Undefined)
                {
                    return "";
                }
                OutputDebugStream("フォント名:" + ret.フォント名.ToString());
                return ret.フォント名;
            }
            catch (Exception e)
            {
                OutputDebugStream("onフォント要求時Error:" + e.Message);
            }
            return "";
        }

        public static String on音楽要求時(String filepath)
        {
            try
            {
                dynamic arg = new ExpandoObject();
                dynamic ret = new ExpandoObject();
                arg.ファイル名 = filepath;
                engine.Script.on音楽要求時(arg, ret);
                if (ret is Undefined)
                {
                    return "";
                }
                if (ret.ファイル名 is Undefined)
                {
                    return "";
                }
                return ret.ファイル名;
            }
            catch (Exception e)
            {
                OutputDebugStream("on音楽要求時Error:" + e.Message);
            }
            return "";
        }

        public static String on効果音要求時(String filepath)
        {
            try
            {
                dynamic arg = new ExpandoObject();
                dynamic ret = new ExpandoObject();
                arg.ファイル名 = filepath;
                engine.Script.on効果音要求時(arg, ret);
                if (ret is Undefined)
                {
                    return "";
                }
                if (ret.ファイル名 is Undefined)
                {
                    return "";
                }
                return ret.ファイル名;
            }
            catch (Exception e)
            {
                OutputDebugStream("on効果音要求時Error:" + e.Message);
            }
            return "";
        }

        public static String on顔画像要求時(int iKaoID)
        {
            try
            {
                dynamic arg = new ExpandoObject();
                dynamic ret = new ExpandoObject();
                arg.画像番号 = iKaoID;
                engine.Script.on顔画像要求時(arg, ret);
                if (ret is Undefined)
                {
                    return "";
                }
                if (ret.ファイル名 is Undefined)
                {
                    return "";
                }
                return ret.ファイル名;
            }
            catch (Exception e)
            {
                OutputDebugStream("on顔画像要求時Error:" + e.Message);
            }
            return "";
        }

        public static String on姫顔画像要求時(int iKaoID)
        {
            try
            {
                dynamic arg = new ExpandoObject();
                dynamic ret = new ExpandoObject();
                arg.画像番号 = iKaoID;
                engine.Script.on姫顔画像要求時(arg, ret);
                if (ret is Undefined)
                {
                    return "";
                }
                if (ret.ファイル名 is Undefined)
                {
                    return "";
                }
                return ret.ファイル名;
            }
            catch (Exception e)
            {
                OutputDebugStream("on姫顔画像要求時Error:" + e.Message);
            }
            return "";
        }


        public static String on家宝画像要求時(int iKahouPicID)
        {
            try
            {
                dynamic arg = new ExpandoObject();
                dynamic ret = new ExpandoObject();
                arg.画像番号 = iKahouPicID;
                engine.Script.on家宝画像要求時(arg, ret);
                if (ret is Undefined)
                {
                    return "";
                }
                if (ret.ファイル名 is Undefined)
                {
                    return "";
                }
                return ret.ファイル名;
            }
            catch (Exception e)
            {
                OutputDebugStream("on家宝画像要求時Error:" + e.Message);
            }
            return "";
        }

        public static String on家紋画像要求時(int iKamonID)
        {
            try
            {
                dynamic arg = new ExpandoObject();
                dynamic ret = new ExpandoObject();
                arg.画像番号 = iKamonID;
                engine.Script.on家紋画像要求時(arg);
                if (ret is Undefined)
                {
                    return "";
                }
                if (ret.ファイル名 is Undefined)
                {
                    return "";
                }
                return ret.ファイル名;
            }
            catch (Exception e)
            {
                OutputDebugStream("on家紋画像要求時Error:" + e.Message);
            }
            return "";
        }


        public static String onファイル要求時(string filename)
        {
            try
            {
                dynamic arg = new ExpandoObject();
                dynamic ret = new ExpandoObject();
                arg.ファイル名 = filename;
                engine.Script.onファイル要求時(arg, ret);
                if (ret is Undefined)
                {
                    return "";
                }
                if (ret.ファイル名 is Undefined)
                {
                    return "";
                }
                return ret.ファイル名;
            }
            catch (Exception e)
            {
                OutputDebugStream("onファイル要求時Error:" + e.Message);
            }
            return "";
        }

        private static bool CreateScope()
        {
            if (engine == null)
            {
                try
                {

                    engine = new V8ScriptEngine(V8ScriptEngineFlags.EnableDynamicModuleImports);

                    engine.DefaultAccess = ScriptAccess.Full;
                    engine.AllowReflection = true;
                    engine.DocumentSettings.AccessFlags = DocumentAccessFlags.EnableAllLoading;

                    engine.AddHostObject("clr", new HostTypeCollection("mscorlib", "System", "System.Core"));
                    engine.AddHostObject("", HostItemFlags.GlobalMembers, new HostTypeCollection("mscorlib", "System", "System.Core"));
                    engine.AddHostObject("host", new ExtendedHostFunctions());
                    rpdobj = new 烈風伝();
                    engine.AddHostType("烈風伝", typeof(烈風伝));
                    console = new JSConsole();
                    engine.AddHostType("console", typeof(JSConsole));
                    // engine.Execute(new DocumentInfo { Category = ModuleCategory.CommonJS }, "globalThis.require = require");
                    engine.Execute(new DocumentInfo { Category = ModuleCategory.CommonJS }, "globalThis.require = require");
                }
                catch (Exception e)
                {
                    OutputDebugStream("エンジン失敗 ?" + e.Message);
                }
            }

            if (engine != null)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        private static bool DoString(String expression, String inAction = "DoString")
        {
            if (CreateScope() == false)
            {
                return false;
            }

            try
            {

                // 文字列からソース生成
                engine.Evaluate(expression);
                return true;
            }
            catch (ScriptEngineException e)
            {
                OutputDebugStream("in " + inAction);
                var stack = e.GetScriptStack();
                OutputDebugStream(stack.ToString());

                /*
                var errorLocation = engine.GetErrorLocation(error);
                if (!string.IsNullOrWhiteSpace(errorLocation))
                {
                    message += "\n" + errorLocation;
                }

                var stackTrace = WindowsScriptEngine.
                if (!string.IsNullOrWhiteSpace(stackTrace))
                {
                    message += "\n" + stackTrace;
                }

                return message;
                */

                ScriptEngineException next = e.InnerException as ScriptEngineException;
                while (next != null)
                {
                    /*
                    OutputDebugStream(next.Data.ToString());
                    OutputDebugStream(next.Source.ToString());
                     */
                    OutputDebugStream(next.ErrorDetails);
                    next = next.InnerException as ScriptEngineException;
                }
            }
            catch (ScriptInterruptedException e)
            {
                OutputDebugStream("in " + inAction);
                var stack = e.GetScriptStack();
                OutputDebugStream(stack.ToString());


                ScriptInterruptedException next = e.InnerException as ScriptInterruptedException;
                while (next != null)
                {
                    OutputDebugStream(next.Data.ToString());
                    OutputDebugStream(next.Source.ToString());
                    OutputDebugStream(next.ErrorDetails);
                    next = next.InnerException as ScriptInterruptedException;
                }
            }
            catch (Exception e)
            {
                OutputDebugStream("in " + inAction);
                OutputDebugStream(e.GetScriptStack());
            }

            return false;
        }


        public static bool DoFile(String filename)
        {
            if (CreateScope() == false)
            {
                return false;
            }

            try
            {
                using (System.IO.StreamReader sr = new System.IO.StreamReader(filename, System.Text.Encoding.UTF8))
                {
                    String expression = sr.ReadToEnd();
                    sr.Close();
                    return DoString(expression, filename);
                }
            }
            catch (Exception e)
            {
                OutputDebugStream("DoFileError:" + e.Message);
            }
            return false;
        }

        private static void DestroyEngine()
        {
            if (engine != null)
            {
                engine = null;
            }
        }
    }

}