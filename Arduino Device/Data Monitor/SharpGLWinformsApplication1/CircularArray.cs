using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SharpGLWinformsApplication1
{
    class CircularArray
    {
        private static int MAXSIZE;
        private int ELEMENTNUM;
        private int HEAD=0;
        private int TAIL=0;
        private int[] Array;
        public int newest;
        public CircularArray(int maxSize)
        {
            MAXSIZE=maxSize;
            Array = new int[MAXSIZE];
            HEAD=0;
            TAIL=0;
            ELEMENTNUM=0;
            for (int i = 0; i < Array.Length;i++ )
            {
                Array[i] = 0;
            }
        }
        public void addValue(int Value)
        {
            if(HEAD!=TAIL)
            {
               Array[TAIL]=newest=Value;
                TAIL++;
                ELEMENTNUM++;
            }
            else
            {
                Array[HEAD]=newest=Value;
                if(ELEMENTNUM!=0)
                    HEAD++;
                else
                    TAIL++;
                ELEMENTNUM++;
            }
            if(HEAD==MAXSIZE)
                HEAD=0;
            if(TAIL==MAXSIZE)
                TAIL=0;
        }

        public int getValue(int index)
        {
            int i = index + HEAD;
            if (i >= MAXSIZE)
                i -= MAXSIZE;
            return Array[i];
        }

        public void empty()
        {
            for (int i = 0; i < ELEMENTNUM; i++)
                Array[i] = 0;
            ELEMENTNUM = 0;
            HEAD = 0;
            TAIL = 0;
        }
    }
}
