﻿/*
 * Copyright (c) 2023-2024 天翔記.jp
 * under the MIT license
 */

using System;
using System.Collections.Generic;

// Consoleのチープクラス
namespace ゲーム
{
    public sealed class JSConsole
    {
        public static void log(params Object[] expressions)
        {
            StaticLib.debuginfo(FormatObjects(expressions));
        }

        public static void assert()
        {
            StaticLib.debuginfo("Assertion failed: console.assert");
        }


        /// <summary>
        /// console.log系が搭載しているformat相当。
        /// console.logは元々sprintf系の全部を実装しているわけではなく、ほんの一部だけ持っている。
        ///  %s	 String
        ///  %d, %i	 Integer
        ///  %f	 Floating point number
        ///  %o	 Object hyperlink
        /// </summary>
        /// <param name="items"> 書式化指定子の引数達。但し、数が不足していたり、数が多かったりしてもOK </param>
        /// <returns> An array containing formatted strings interspersed with objects. </returns>
        private static object[] FormatObjects(object[] items)
        {
            if (items.Length == 0)
            {
                return new object[0];
            }
            var result = new List<object>();
            var formattedString = new System.Text.StringBuilder();

            // 最初の引数が文字列なら, 書式化指定子を含んでいる(かもしれない)とされる.
            int itemsConsumed = 1;
            if (items[0] is string)
            {
                string formatString = (string)items[0];

                int previousPatternIndex = 0, patternIndex;
                while (items.Length > itemsConsumed)
                {
                    // %の書式化の発見.
                    patternIndex = formatString.IndexOf('%', previousPatternIndex);
                    if (patternIndex == -1 || patternIndex == formatString.Length - 1)
                    {
                        break;
                    }

                    // %含まないならそのまま足しこみ.
                    formattedString.Append(formatString, previousPatternIndex, patternIndex - previousPatternIndex);

                    // パターンのタイプ.
                    char patternType = formatString[patternIndex + 1];

                    // 置き換え文字列を決定する
                    string replacement;
                    switch (patternType)
                    {
                        case 's':
                            replacement = items[itemsConsumed].ToString();
                            itemsConsumed++;
                            break;
                        case 'd':
                        case 'i':
                            replacement = JSTypeConverter.ToInteger(items[itemsConsumed]).ToString();
                            itemsConsumed++;
                            break;
                        case 'f':
                            replacement = JSTypeConverter.ToDouble(items[itemsConsumed]).ToString();
                            itemsConsumed++;
                            break;
                        case '%':
                            replacement = "%";
                            break;
                        case 'o':
                            replacement = string.Empty;
                            if (formattedString.Length > 0)
                                result.Add(formattedString.ToString());
                            result.Add(items[itemsConsumed]);
                            formattedString.Remove(0, formattedString.Length);
                            itemsConsumed++;
                            break;
                        default:
                            replacement = "%" + patternType;
                            break;
                    }

                    // パターンで置き換えたものを出す.
                    formattedString.Append(replacement);

                    // パターンは文字分なので、次のサーチのために２文字進める.
                    previousPatternIndex = patternIndex + 2;
                }

                // 結果に%含まないなら、そのまま足しこみ.
                formattedString.Append(formatString, previousPatternIndex, formatString.Length - previousPatternIndex);

                // 書式化で整った文字列を配列へと足し込む.
                if (formattedString.Length > 0)
                {
                    result.Add(formattedString.ToString());
                }

                // パターンによって消費されなかった配列は、そのまま足しこんでおく。C系のprintfと違って数が足りなかったりしたからといってぶっ飛ぶことはない.
                for (int i = itemsConsumed; i < items.Length; i++)
                {
                    result.Add(items[i]);
                }
                return result.ToArray();
            }
            else
            {
                // 最初が文字列でないなら、この関数でやることはないい。配列郡全体を何も変化させず、そのまま返す。
                return items;
            }
        }

    }


    internal sealed class JSTypeConverter
    {
        public static Object ToInteger(Object value)
        {
            // 非数値(+の無限と-の無限を足したり、など)
            if (value is Double)
            {
                double d = (double)value;
                if (Double.IsNaN(d))
                {
                    return Double.NaN;
                }
                // プラスの無限
                if (Double.IsPositiveInfinity(d))
                {
                    return "Infinity";
                }
                // マイナスの無限
                if (Double.IsNegativeInfinity(d))
                {
                    return "-Infinity";
                }
            }

            Object result = new Object();

            // 32bit
            if (IntPtr.Size == 4)
            {
                // まずは整数でトライ
                Int32 itmp = 0;
                bool success = Int32.TryParse(value.ToString(), out itmp);

                if (success == true)
                {
                    result = itmp;
                }

                else
                {
                    // 次に少数でトライ
                    Double dtmp = 0;
                    success = Double.TryParse(value.ToString(), out dtmp);
                    if (success)
                    {
                        // ★★★JScriptエンジンやFirefoxは、%dでは0に近づける方にまるめる
                        result = (Int32)(dtmp);
                    }

                    else
                    {
                        result = value;
                    }
                }
            }

            // 64bit
            else
            {
                // まずは整数でトライ
                Int64 itmp = 0;
                bool success = Int64.TryParse(value.ToString(), out itmp);

                if (success == true)
                {
                    result = itmp;
                }

                else
                {
                    // 次に少数でトライ
                    Double dtmp = 0;
                    success = Double.TryParse(value.ToString(), out dtmp);
                    if (success)
                    {
                        // ★★★JScriptエンジンやFirefoxは、%dでは0に近づける方にまるめる
                        result = (Int64)(dtmp);
                    }
                    else
                    {
                        result = value;
                    }
                }
            }

            return result;

        }

        public static Object ToDouble(Object value)
        {
            // 非数値(+の無限と-の無限を足したり、など)
            if (value is Double)
            {
                double d = (double)value;
                if (Double.IsNaN(d))
                {
                    return Double.NaN;
                }
                // プラスの無限
                if (Double.IsPositiveInfinity(d))
                {
                    return "Infinity";
                }
                // マイナスの無限
                if (Double.IsNegativeInfinity(d))
                {
                    return "-Infinity";
                }
            }

            Object result = new Object();

            // 32bit
            if (IntPtr.Size == 4)
            {
                // まずに少数でトライ
                Double dtmp = 0;
                bool success = Double.TryParse(value.ToString(), out dtmp);
                if (success)
                {
                    result = dtmp;
                }
                else
                {
                    // 次に整数でトライ
                    Int32 itmp = 0;
                    success = Int32.TryParse(value.ToString(), out itmp);

                    if (success == true)
                    {
                        result = itmp;
                    }
                    else
                    {
                        result = value;
                    }
                }
            }

            // 64bit
            else
            {
                // まず少数でトライ
                Double dtmp = 0;
                bool success = Double.TryParse(value.ToString(), out dtmp);
                if (success)
                {
                    result = dtmp;
                }

                else
                {
                    // 次に整数でトライ
                    Int64 itmp = 0;
                    success = Int64.TryParse(value.ToString(), out itmp);

                    if (success == true)
                    {
                        result = itmp;
                    }
                    else
                    {
                        result = value;
                    }
                }
            }

            return result;
        }

    }
}